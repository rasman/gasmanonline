#pragma once
// CsvReader.h — header-only parser for the GasMan sections-based CSV format.
//
// Format synopsis
// ---------------
//   # comment lines are ignored
//   blank lines are ignored
//
//   [patient]
//   weight_kg,<value>
//
//   [volumes]
//   vrg,<value>  fat,<value>  ven,<value>  alv,<value>  mus,<value>
//
//   [flows]
//   vrg,<value>  fat,<value>  mus,<value>
//
//   [agent]          ← repeat this entire block for each additional agent
//   name,<string>
//   lambdaMus,<value>
//   lambdaBlood,<value>
//   lambdaFat,<value>
//   volatility,<value>
//   bottleSize,<value>
//   lambdaVrg,<value>
//   bottleCost,<value>
//
//   [settings]
//   va,fgf,circuit,time,co,del1,inject1,del2,inject2,...
//   <row1>
//   ...
//
// Single-agent shorthand: use "del" and "inject" instead of "del1"/"inject1".
// Multiple [agent] sections are allowed; each defines one agent.
// Delivery columns are matched to agents positionally (del1 → first [agent],
// del2 → second [agent], etc.).
//
// All numeric fields are stored as float.  String fields (circuit, time, agent
// name) are stored as std::string.  Missing sections / fields keep their
// default-initialised values (0.0f / "").

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <cctype>

// ---------------------------------------------------------------------------
// Data structures
// ---------------------------------------------------------------------------

struct AgentParams {
    std::string name;
    float lambdaMus    = 0.0f;
    float lambdaBlood  = 0.0f;
    float lambdaFat    = 0.0f;
    float volatility   = 0.0f;
    float bottleSize   = 0.0f;
    float lambdaVrg    = 0.0f;
    float bottleCost   = 0.0f;
};

/// Per-agent delivery values for one time-step.
struct AgentSetting {
    float del    = 0.0f;   ///< vaporiser delivery (% or dial)
    float inject = 0.0f;   ///< bolus injection flag (0 / 1)
};

struct Setting {
    float       va      = 0.0f;
    float       fgf     = 0.0f;
    float       co      = 0.0f;
    std::string circuit;
    std::string time;
    /// One entry per agent, positionally matched to GasManInput::agents.
    std::vector<AgentSetting> agentSettings;
};

struct GasManInput {
    // [patient]
    float weightKg  = 70.0f;

    // [volumes]
    float vrgVol    = 6.0f;
    float fatVol    = 14.5f;
    float venVol    = 1.0f;
    float alvVol    = 2.5f;
    float musVol    = 33.0f;

    // [flows]
    float vrgFlow   = 0.76f;
    float fatFlow   = 0.06f;
    float musFlow   = 0.18f;

    // [agent] — one entry per [agent] section (at least one expected)
    std::vector<AgentParams> agents;

    // [settings]
    std::vector<Setting> settings;
};

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------
namespace csv_detail {

inline std::string trim(const std::string& s)
{
    auto lo = std::find_if(s.begin(), s.end(),
                           [](unsigned char c){ return !std::isspace(c); });
    auto hi = std::find_if(s.rbegin(), s.rend(),
                           [](unsigned char c){ return !std::isspace(c); }).base();
    return (lo < hi) ? std::string(lo, hi) : std::string{};
}

/// Split a line on commas; individual fields are trimmed.
inline std::vector<std::string> split_csv(const std::string& line)
{
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string tok;
    while (std::getline(ss, tok, ','))
        parts.push_back(trim(tok));
    return parts;
}

inline float to_float(const std::string& s, float fallback = 0.0f)
{
    if (s.empty()) return fallback;
    try { return std::stof(s); }
    catch (...) { return fallback; }
}

inline std::string get_col(const std::vector<std::string>& row, int idx)
{
    if (idx < 0 || idx >= static_cast<int>(row.size())) return "";
    return row[idx];
}

/// Column-index map for a [settings] header row.
/// del[i] / inject_[i] hold the column index for agent (i+1).
struct SettingColIdx {
    int va      = -1;
    int fgf     = -1;
    int circuit = -1;
    int time    = -1;
    int co      = -1;
    std::vector<int> del;     ///< del[i] = column index for agent i (0-based)
    std::vector<int> inject_; ///< inject_[i] = column index for agent i
};

inline SettingColIdx parse_setting_header(const std::vector<std::string>& hdr)
{
    SettingColIdx idx;
    for (int i = 0; i < static_cast<int>(hdr.size()); ++i) {
        std::string h = hdr[i];
        std::transform(h.begin(), h.end(), h.begin(),
                       [](unsigned char c){ return std::tolower(c); });

        if      (h == "va")      { idx.va      = i; }
        else if (h == "fgf")     { idx.fgf     = i; }
        else if (h == "circuit") { idx.circuit = i; }
        else if (h == "time")    { idx.time    = i; }
        else if (h == "co")      { idx.co      = i; }

        // "del" (no suffix) = first agent; "del1" = first, "del2" = second, …
        else if (h == "del") {
            if (idx.del.empty()) idx.del.assign(1, -1);
            idx.del[0] = i;
        }
        else if (h == "inject") {
            if (idx.inject_.empty()) idx.inject_.assign(1, -1);
            idx.inject_[0] = i;
        }
        else if (h.size() > 3 && h.substr(0, 3) == "del") {
            try {
                int n = std::stoi(h.substr(3));   // 1-indexed in the CSV
                if (n >= 1) {
                    int ai = n - 1;
                    if (static_cast<int>(idx.del.size()) <= ai)
                        idx.del.resize(ai + 1, -1);
                    idx.del[ai] = i;
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
                    idx.inject_[ai] = i;
                }
            } catch (...) {}
        }
    }
    return idx;
}

} // namespace csv_detail

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

/// Parse a GasMan sections-based CSV file and return a GasManInput struct.
/// Throws std::runtime_error on I/O failure.
inline GasManInput parseCsvFile(const std::string& path)
{
    std::ifstream f(path);
    if (!f)
        throw std::runtime_error("Cannot open CSV file: " + path);

    GasManInput inp;

    enum class Section { None, Patient, Volumes, Flows, Agent, Settings };
    Section cur = Section::None;

    bool settingsHeaderDone = false;
    csv_detail::SettingColIdx settingIdx;

    std::string line;
    while (std::getline(f, line)) {
        // Strip CR for Windows line-endings on non-Windows platforms.
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        std::string trimmed = csv_detail::trim(line);
        if (trimmed.empty() || trimmed[0] == '#')
            continue;

        // Section header?
        if (trimmed.front() == '[' && trimmed.back() == ']') {
            std::string sec = csv_detail::trim(trimmed.substr(1, trimmed.size() - 2));
            std::transform(sec.begin(), sec.end(), sec.begin(),
                           [](unsigned char c){ return std::tolower(c); });

            if      (sec == "patient")  { cur = Section::Patient; }
            else if (sec == "volumes")  { cur = Section::Volumes; }
            else if (sec == "flows")    { cur = Section::Flows;   }
            else if (sec == "agent")    {
                cur = Section::Agent;
                inp.agents.push_back(AgentParams{});  // start a new agent entry
            }
            else if (sec == "settings") {
                cur = Section::Settings;
                settingsHeaderDone = false;
            }
            else { cur = Section::None; }
            continue;
        }

        auto cols = csv_detail::split_csv(trimmed);
        if (cols.empty()) continue;

        switch (cur) {
        case Section::Patient:
            if (cols.size() >= 2) {
                std::string key = cols[0];
                std::transform(key.begin(), key.end(), key.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                if (key == "weight_kg")
                    inp.weightKg = csv_detail::to_float(cols[1]);
            }
            break;

        case Section::Volumes:
            if (cols.size() >= 2) {
                std::string key = cols[0];
                std::transform(key.begin(), key.end(), key.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                if      (key == "vrg") inp.vrgVol = csv_detail::to_float(cols[1]);
                else if (key == "fat") inp.fatVol = csv_detail::to_float(cols[1]);
                else if (key == "ven") inp.venVol = csv_detail::to_float(cols[1]);
                else if (key == "alv") inp.alvVol = csv_detail::to_float(cols[1]);
                else if (key == "mus") inp.musVol = csv_detail::to_float(cols[1]);
            }
            break;

        case Section::Flows:
            if (cols.size() >= 2) {
                std::string key = cols[0];
                std::transform(key.begin(), key.end(), key.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                if      (key == "vrg") inp.vrgFlow = csv_detail::to_float(cols[1]);
                else if (key == "fat") inp.fatFlow = csv_detail::to_float(cols[1]);
                else if (key == "mus") inp.musFlow = csv_detail::to_float(cols[1]);
            }
            break;

        case Section::Agent:
            if (!inp.agents.empty() && cols.size() >= 2) {
                AgentParams& a = inp.agents.back();
                std::string key = cols[0];
                std::transform(key.begin(), key.end(), key.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                if      (key == "name")        a.name        = cols[1];
                else if (key == "lambdamus")   a.lambdaMus   = csv_detail::to_float(cols[1]);
                else if (key == "lambdablood") a.lambdaBlood = csv_detail::to_float(cols[1]);
                else if (key == "lambdafat")   a.lambdaFat   = csv_detail::to_float(cols[1]);
                else if (key == "volatility")  a.volatility  = csv_detail::to_float(cols[1]);
                else if (key == "bottlesize")  a.bottleSize  = csv_detail::to_float(cols[1]);
                else if (key == "lambdavrg")   a.lambdaVrg   = csv_detail::to_float(cols[1]);
                else if (key == "bottlecost")  a.bottleCost  = csv_detail::to_float(cols[1]);
            }
            break;

        case Section::Settings:
            if (!settingsHeaderDone) {
                settingIdx = csv_detail::parse_setting_header(cols);
                settingsHeaderDone = true;
            } else {
                Setting s;
                s.va      = csv_detail::to_float(csv_detail::get_col(cols, settingIdx.va));
                s.fgf     = csv_detail::to_float(csv_detail::get_col(cols, settingIdx.fgf));
                s.co      = csv_detail::to_float(csv_detail::get_col(cols, settingIdx.co));
                s.circuit = csv_detail::get_col(cols, settingIdx.circuit);
                s.time    = csv_detail::get_col(cols, settingIdx.time);

                // Build per-agent delivery vector.
                size_t nAgents = std::max(settingIdx.del.size(),
                                          settingIdx.inject_.size());
                if (nAgents == 0) nAgents = 1;  // backward compat: treat bare del/inject as agent 1
                s.agentSettings.resize(nAgents);
                for (size_t ai = 0; ai < nAgents; ++ai) {
                    if (ai < settingIdx.del.size() && settingIdx.del[ai] >= 0)
                        s.agentSettings[ai].del =
                            csv_detail::to_float(csv_detail::get_col(cols, settingIdx.del[ai]));
                    if (ai < settingIdx.inject_.size() && settingIdx.inject_[ai] >= 0)
                        s.agentSettings[ai].inject =
                            csv_detail::to_float(csv_detail::get_col(cols, settingIdx.inject_[ai]));
                }
                inp.settings.push_back(std::move(s));
            }
            break;

        case Section::None:
        default:
            break;
        }
    }

    return inp;
}
