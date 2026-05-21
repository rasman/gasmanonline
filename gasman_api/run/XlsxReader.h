#pragma once
// XlsxReader.h — minimal XLSX → GasManInput reader.
//
// An XLSX file is a ZIP archive.  We decompress it with miniz (single-header),
// then parse two XML files inside it:
//   xl/sharedStrings.xml  — pool of all string values
//   xl/worksheets/sheet1.xml — cell data; string cells reference the pool by index
//
// Layout expected (matches gasman_template.xlsx, mirrors python_gas.py):
//
//   Row containing "Patient - Weight"
//       +1  col C (2)  → weight_kg
//       +3  col B (1)  → vrg volume
//       +4  col B      → fat volume
//       +5  col B      → ven volume
//       +6  col B      → alv volume
//       +7  col B      → mus volume
//       +9  col B      → vrg flow
//       +10 col B      → fat flow
//       +11 col B      → mus flow
//
//   Row containing "Agents"
//       +1 row         → column header row (name, lambdaMus, lambdaBlood, …)
//       +2 row         → first agent data row
//
//   Row containing "Settings"
//       +1 row         → column header row (va, fgf, circuit, time, co, del, inject)
//       +2 row …       → setting data rows
//
// Time values in Excel are stored as a decimal fraction of a day
// (e.g. 5 minutes = 5/1440 ≈ 0.003472); we convert to "HH:MM:SS".

// ── miniz 2.2.0: ZIP + deflate (MIT licence) ─────────────────────────────
// miniz.h declares the API; miniz.c (compiled separately) provides the
// implementation.  No MINIZ_IMPLEMENTATION trick needed in this version.
#include "miniz.h"

#include "CsvReader.h"    // GasManInput, AgentParams, Setting

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <cctype>

// ─────────────────────────────────────────────────────────────────────────────
// Internal helpers
// ─────────────────────────────────────────────────────────────────────────────
namespace xlsx_detail {

// ── String utilities ─────────────────────────────────────────────────────────

inline std::string toLower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}

inline std::string stripSpaces(std::string s)
{
    s.erase(std::remove_if(s.begin(), s.end(),
            [](unsigned char c){ return std::isspace(c); }), s.end());
    return s;
}

// ── Minimal XML attribute/text extractor ──────────────────────────────────

/// Return the value of attribute 'attrName' from an opening tag string.
/// E.g. attrValue("<c r=\"B3\" t=\"s\">", "r") → "B3"
inline std::string attrValue(const std::string& tag, const std::string& name)
{
    std::string needle = " " + name + "=\"";
    size_t p = tag.find(needle);
    if (p == std::string::npos) return {};
    p += needle.size();
    size_t e = tag.find('"', p);
    return (e != std::string::npos) ? tag.substr(p, e - p) : std::string{};
}

// ── Shared strings parser ─────────────────────────────────────────────────

/// Build the shared-string table from xl/sharedStrings.xml.
/// Each <si> entry maps to one string (concatenating all <t> runs inside it).
inline std::vector<std::string> parseSharedStrings(const std::string& xml)
{
    std::vector<std::string> table;
    size_t pos = 0;

    while (pos < xml.size()) {
        // Find next <si> or <si>
        size_t siOpen = xml.find("<si", pos);
        if (siOpen == std::string::npos) break;

        // Find its closing tag
        size_t siClose = xml.find("</si>", siOpen);
        if (siClose == std::string::npos) break;

        // Within this <si>…</si>, concatenate all <t> text runs.
        std::string entry;
        size_t cur = siOpen;
        while (cur < siClose) {
            size_t tOpen = xml.find("<t", cur);
            if (tOpen == std::string::npos || tOpen >= siClose) break;

            // Skip closing tags </t>
            if (tOpen + 2 < xml.size() && xml[tOpen + 2] == '/') {
                cur = tOpen + 3;
                continue;
            }
            size_t gt = xml.find('>', tOpen);
            if (gt == std::string::npos || gt >= siClose) break;

            size_t textStart = gt + 1;
            size_t tClose    = xml.find("</t>", textStart);
            if (tClose == std::string::npos || tClose > siClose) break;

            entry += xml.substr(textStart, tClose - textStart);
            cur = tClose + 4;
        }

        table.push_back(entry);
        pos = siClose + 5;
    }
    return table;
}

// ── Cell reference parsing ────────────────────────────────────────────────

/// "A" → 0, "B" → 1, "Z" → 25, "AA" → 26, …  (0-based)
inline int colLettersToIndex(const std::string& letters)
{
    int v = 0;
    for (char c : letters)
        v = v * 26 + (std::toupper(static_cast<unsigned char>(c)) - 'A' + 1);
    return v - 1;
}

/// "A1" → (row=0, col=0);  "B3" → (row=2, col=1).  Returns false on bad input.
inline bool parseCellRef(const std::string& ref, int& row, int& col)
{
    size_t i = 0;
    while (i < ref.size() && std::isalpha(static_cast<unsigned char>(ref[i]))) ++i;
    if (i == 0 || i == ref.size()) return false;
    col = colLettersToIndex(ref.substr(0, i));
    try { row = std::stoi(ref.substr(i)) - 1; }
    catch (...) { return false; }
    return true;
}

// ── Worksheet parser ──────────────────────────────────────────────────────

using Grid = std::map<int /*row*/, std::map<int /*col*/, std::string>>;

/// Build a (row→col→value) grid from xl/worksheets/sheet1.xml.
inline Grid parseWorksheet(const std::string& xml,
                            const std::vector<std::string>& sharedStrings)
{
    Grid grid;
    size_t pos = 0;

    while (pos < xml.size()) {
        // Locate next <c (cell) element
        size_t cStart = xml.find("<c ", pos);
        if (cStart == std::string::npos) break;

        size_t gtPos = xml.find('>', cStart);
        if (gtPos == std::string::npos) break;

        std::string cTag = xml.substr(cStart, gtPos - cStart + 1);
        bool selfClose   = (!cTag.empty() && cTag.back() == '/');
        //  OR  the char before '>' is '/'
        if (!selfClose && gtPos > 0 && xml[gtPos - 1] == '/')
            selfClose = true;

        std::string cellRef    = attrValue(cTag, "r");
        bool isStr             = (cTag.find("t=\"s\"")          != std::string::npos);
        bool isBool            = (cTag.find("t=\"b\"")          != std::string::npos);
        bool isInlineStr       = (cTag.find("t=\"inlineStr\"")  != std::string::npos);

        if (!cellRef.empty() && !selfClose) {
            // Find the closing </c>
            size_t cEnd = xml.find("</c>", gtPos);
            if (cEnd == std::string::npos) break;

            std::string cellBody = xml.substr(gtPos + 1, cEnd - gtPos - 1);
            std::string val;

            if (isInlineStr) {
                // Inline string: value is in <is><t>…</t></is> (possibly multiple <t> runs)
                size_t cur = 0;
                while (cur < cellBody.size()) {
                    size_t tOpen = cellBody.find("<t", cur);
                    if (tOpen == std::string::npos) break;
                    // Skip closing tags </t>
                    if (tOpen + 2 < cellBody.size() && cellBody[tOpen + 2] == '/') {
                        cur = tOpen + 3; continue;
                    }
                    size_t gt = cellBody.find('>', tOpen);
                    if (gt == std::string::npos) break;
                    size_t tClose = cellBody.find("</t>", gt + 1);
                    if (tClose == std::string::npos) break;
                    val += cellBody.substr(gt + 1, tClose - gt - 1);
                    cur = tClose + 4;
                }
                int row, col;
                if (!val.empty() && parseCellRef(cellRef, row, col))
                    grid[row][col] = val;
            } else {
                // Shared string, boolean, or numeric: value is in <v>…</v>
                size_t vOpen  = cellBody.find("<v>");
                size_t vClose = cellBody.find("</v>");

                if (vOpen != std::string::npos && vClose != std::string::npos) {
                    val = cellBody.substr(vOpen + 3, vClose - vOpen - 3);

                    if (isStr) {
                        // Shared string index
                        try {
                            int idx = std::stoi(val);
                            val = (idx >= 0 && idx < static_cast<int>(sharedStrings.size()))
                                  ? sharedStrings[idx] : "";
                        } catch (...) { val = ""; }
                    } else if (isBool) {
                        val = (val == "1") ? "TRUE" : "FALSE";
                    }
                    // Else: numeric / date value — keep as-is

                    int row, col;
                    if (parseCellRef(cellRef, row, col))
                        grid[row][col] = val;
                }
            }
            pos = cEnd + 4;
        } else {
            pos = gtPos + 1;
        }
    }
    return grid;
}

// ── Grid accessors ────────────────────────────────────────────────────────

inline std::string getCell(const Grid& g, int row, int col)
{
    auto r = g.find(row);
    if (r == g.end()) return {};
    auto c = r->second.find(col);
    return (c != r->second.end()) ? c->second : std::string{};
}

inline float getCellF(const Grid& g, int row, int col, float def = 0.0f)
{
    std::string s = getCell(g, row, col);
    if (s.empty()) return def;
    try { return std::stof(s); } catch (...) { return def; }
}

/// Return the first row index (0-based) whose any cell contains keyword
/// (case-insensitive).  Returns -1 if not found.
inline int findRow(const Grid& g, const std::string& keyword)
{
    std::string kl = toLower(keyword);
    for (auto& [r, cols] : g)
        for (auto& [c, val] : cols)
            if (toLower(val).find(kl) != std::string::npos)
                return r;
    return -1;
}

// ── Settings header ───────────────────────────────────────────────────────

struct SettingIdx {
    int va=-1, fgf=-1, circuit=-1, time_=-1, co=-1;
    std::vector<int> del_;    ///< del_[i] = column index for agent i (0-based)
    std::vector<int> inject_; ///< inject_[i] = column index for agent i
};

inline SettingIdx parseSettingHeader(const Grid& g, int headerRow)
{
    SettingIdx idx;
    auto r = g.find(headerRow);
    if (r == g.end()) return idx;
    for (auto& [col, val] : r->second) {
        std::string h = stripSpaces(toLower(val));
        if      (h == "va")      idx.va      = col;
        else if (h == "fgf")     idx.fgf     = col;
        else if (h == "circuit") idx.circuit = col;
        else if (h == "time")    idx.time_   = col;
        else if (h == "co")      idx.co      = col;
        // "del" / "inject" (no suffix) → first agent
        else if (h == "del") {
            if (idx.del_.empty()) idx.del_.assign(1, -1);
            idx.del_[0] = col;
        }
        else if (h == "inject") {
            if (idx.inject_.empty()) idx.inject_.assign(1, -1);
            idx.inject_[0] = col;
        }
        // "del1", "del2", … / "inject1", "inject2", …
        else if (h.size() > 3 && h.substr(0, 3) == "del") {
            try {
                int n = std::stoi(h.substr(3));
                if (n >= 1) {
                    int ai = n - 1;
                    if (static_cast<int>(idx.del_.size()) <= ai)
                        idx.del_.resize(ai + 1, -1);
                    idx.del_[ai] = col;
                }
            } catch (...) {}
        }
        else if (h.size() > 6 && h.substr(0, 6) == "inject") {
            try {
                int n = std::stoi(h.substr(6));
                if (n >= 1) {
                    int ai = n - 1;
                    if (static_cast<int>(idx.inject_.size()) <= ai)
                        idx.inject_.resize(ai + 1, -1);
                    idx.inject_[ai] = col;
                }
            } catch (...) {}
        }
    }
    return idx;
}

// ── Time conversion ───────────────────────────────────────────────────────

/// Excel stores time as a fraction of a day.  Convert to "HH:MM:SS".
inline std::string fractionToTimeStr(double frac)
{
    // Clamp to [0, 1)
    frac -= std::floor(frac);
    int totalSec = static_cast<int>(std::round(frac * 86400.0));
    int hh = totalSec / 3600;
    int mm = (totalSec % 3600) / 60;
    int ss = totalSec % 60;
    char buf[12];
    std::snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hh, mm, ss);
    return buf;
}

/// A cell value from a time-formatted column might be:
///   - Already "HH:MM:SS" (string, t="s")
///   - A decimal fraction  (numeric, no t attribute)
inline std::string asTimeString(const std::string& raw)
{
    if (raw.empty()) return "00:00:00";
    // Check if it looks like HH:MM or HH:MM:SS already
    if (raw.size() >= 5 && raw[2] == ':') {
        // Normalise HH:MM → HH:MM:SS; the engine requires exactly 3 components
        if (raw.size() == 5) return raw + ":00";
        return raw;
    }
    // Try numeric fraction
    try {
        double v = std::stod(raw);
        return fractionToTimeStr(v);
    } catch (...) {
        return raw;   // return as-is
    }
}

// ── ZIP extraction ────────────────────────────────────────────────────────

/// Extract a named file from an open miniz archive as a std::string.
inline std::string extractEntry(mz_zip_archive& zip, const char* name)
{
    int idx = mz_zip_reader_locate_file(&zip, name, nullptr, 0);
    if (idx < 0)
        throw std::runtime_error(std::string("XLSX: cannot find entry: ") + name);

    size_t size = 0;
    void*  data = mz_zip_reader_extract_to_heap(&zip, static_cast<mz_uint>(idx), &size, 0);
    if (!data)
        throw std::runtime_error(std::string("XLSX: decompress failed: ") + name);

    std::string result(static_cast<char*>(data), size);
    mz_free(data);
    return result;
}

} // namespace xlsx_detail

// ─────────────────────────────────────────────────────────────────────────────
// Public API
// ─────────────────────────────────────────────────────────────────────────────

/// Parse a GasMan Excel template (.xlsx) and return a GasManInput.
/// Throws std::runtime_error on any failure.
inline GasManInput parseXlsxFile(const std::string& path)
{
    using namespace xlsx_detail;

    // ── Open the ZIP archive ─────────────────────────────────────────────────
    mz_zip_archive zip;
    std::memset(&zip, 0, sizeof(zip));
    if (!mz_zip_reader_init_file(&zip, path.c_str(), 0))
        throw std::runtime_error("XLSX: cannot open file: " + path);

    std::string sharedXml, sheetXml;
    try {
        // sharedStrings.xml is absent when there are no string cells (unlikely
        // for GasMan scenarios, but handle it gracefully).
        try { sharedXml = extractEntry(zip, "xl/sharedStrings.xml"); }
        catch (...) { sharedXml = ""; }

        sheetXml = extractEntry(zip, "xl/worksheets/sheet1.xml");
    } catch (...) {
        mz_zip_reader_end(&zip);
        throw;
    }
    mz_zip_reader_end(&zip);

    // ── Parse into a cell grid ───────────────────────────────────────────────
    auto sharedStrings = parseSharedStrings(sharedXml);
    auto grid          = parseWorksheet(sheetXml, sharedStrings);

    GasManInput inp;   // defaults from CsvReader.h

    // ── Patient / volumes / flows ────────────────────────────────────────────
    // Search for the "Patient - Weight" header row; fall back to just "weight".
    int patRow = findRow(grid, "patient - weight");
    if (patRow < 0) patRow = findRow(grid, "weight");

    if (patRow >= 0) {
        // Layout matches python_gas.py offsets (pandas 0-based → same 0-based here)
        inp.weightKg = getCellF(grid, patRow + 1, 2, inp.weightKg); // row+1, col C

        inp.vrgVol = getCellF(grid, patRow + 3, 1, inp.vrgVol);     // col B
        inp.fatVol = getCellF(grid, patRow + 4, 1, inp.fatVol);
        inp.venVol = getCellF(grid, patRow + 5, 1, inp.venVol);
        inp.alvVol = getCellF(grid, patRow + 6, 1, inp.alvVol);
        inp.musVol = getCellF(grid, patRow + 7, 1, inp.musVol);

        inp.vrgFlow = getCellF(grid, patRow + 9,  1, inp.vrgFlow);
        inp.fatFlow = getCellF(grid, patRow + 10, 1, inp.fatFlow);
        inp.musFlow = getCellF(grid, patRow + 11, 1, inp.musFlow);
    }

    // ── Agents ───────────────────────────────────────────────────────────────
    int agentsRow   = findRow(grid, "agents");
    int settingsRow = findRow(grid, "settings");

    if (agentsRow >= 0 && settingsRow > agentsRow) {
        // agentsRow + 1 = header row;  agentsRow + 2 = first data row
        int headerRow = agentsRow + 1;
        int dataRow   = agentsRow + 2;

        // Build header → column-index map
        std::map<std::string, int> hdr;
        auto rIt = grid.find(headerRow);
        if (rIt != grid.end()) {
            for (auto& [col, val] : rIt->second)
                hdr[stripSpaces(toLower(val))] = col;
        }

        auto get = [&](const std::string& key) {
            auto it = hdr.find(key);
            return (it != hdr.end()) ? getCell(grid, dataRow, it->second) : std::string{};
        };
        auto getF = [&](const std::string& key, float def = 0.f) {
            std::string s = get(key);
            if (s.empty()) return def;
            try { return std::stof(s); } catch (...) { return def; }
        };

        // Read one agent per data row until we reach the Settings section row
        // (or run out of rows).  The first data row is agentsRow + 2.
        int lastAgentRow = (settingsRow > agentsRow) ? settingsRow - 1
                                                     : grid.empty() ? agentsRow + 2
                                                                     : grid.rbegin()->first;
        for (int dataRow = agentsRow + 2; dataRow <= lastAgentRow; ++dataRow) {
            // Skip rows where the "name" cell is empty (blank separator rows).
            auto nameIt = hdr.find("name");
            if (nameIt != hdr.end()) {
                std::string nameVal = getCell(grid, dataRow, nameIt->second);
                if (nameVal.empty()) continue;
            }

            AgentParams a;
            auto getRow = [&](const std::string& key) {
                auto it = hdr.find(key);
                return (it != hdr.end()) ? getCell(grid, dataRow, it->second) : std::string{};
            };
            auto getRowF = [&](const std::string& key, float def = 0.f) {
                std::string s = getRow(key);
                if (s.empty()) return def;
                try { return std::stof(s); } catch (...) { return def; }
            };

            a.name        = getRow("name");
            a.lambdaMus   = getRowF("lambdamus");
            a.lambdaBlood = getRowF("lambdablood");
            a.lambdaFat   = getRowF("lambdafat");
            a.volatility  = getRowF("volatility");
            a.bottleSize  = getRowF("bottlesize");
            a.lambdaVrg   = getRowF("lambdavrg");
            a.bottleCost  = getRowF("bottlecost");
            inp.agents.push_back(a);
        }
    }

    // ── Settings ─────────────────────────────────────────────────────────────
    if (settingsRow >= 0) {
        // settingsRow + 1 = header;  settingsRow + 2 .. end = data rows
        SettingIdx idx = parseSettingHeader(grid, settingsRow + 1);

        // Walk every row after the header until we hit an empty va cell
        int maxRow = grid.empty() ? 0 : grid.rbegin()->first;
        for (int r = settingsRow + 2; r <= maxRow; ++r) {
            // Stop at the first completely empty row in the va column
            if (idx.va >= 0 && getCell(grid, r, idx.va).empty()) continue;

            Setting s;
            s.va      = getCellF(grid, r, idx.va);
            s.fgf     = getCellF(grid, r, idx.fgf);
            s.co      = getCellF(grid, r, idx.co);
            s.circuit = getCell(grid, r, idx.circuit);
            s.time    = asTimeString(getCell(grid, r, idx.time_));

            // Per-agent delivery values.
            size_t nAgents = std::max(idx.del_.size(), idx.inject_.size());
            if (nAgents == 0) nAgents = 1;
            s.agentSettings.resize(nAgents);
            for (size_t ai = 0; ai < nAgents; ++ai) {
                if (ai < idx.del_.size() && idx.del_[ai] >= 0)
                    s.agentSettings[ai].del = getCellF(grid, r, idx.del_[ai]);
                if (ai < idx.inject_.size() && idx.inject_[ai] >= 0)
                    s.agentSettings[ai].inject = getCellF(grid, r, idx.inject_[ai]);
            }

            inp.settings.push_back(s);
        }
    }

    return inp;
}
