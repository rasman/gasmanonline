// gasman_run — standalone GasMan simulation runner.
//
// Reads a scenario from an XML, JSON, CSV, or Excel file and writes simulated
// pharmacokinetic data as CSV.  No Python required.
//
// Usage:
//   gasman_run <input.[xml|json|csv|xlsx]> [options]
//
// Options:
//   --start  <sec>   Simulation start time in seconds     (default 0)
//   --end    <sec>   Simulation end time in seconds       (default: auto — last setting time)
//   --every  <sec>   Output row interval in seconds       (default 10)
//   --output <file>  Write CSV to <file> instead of stdout
//   --ini    <file>  Path to gasman.ini                   (default: auto)
//
// Supported input formats (detected from file extension):
//   .xml   — native GasMan XML scenario
//   .json  — GasMan JSON scenario
//   .csv   — sections-based scenario CSV  (see examples/simulation_template.csv)
//   .xlsx  — Excel workbook  (see examples/gasman_template.xlsx)
//   .gas   — not supported; open in GasMan and export as XML or JSON first
//
// Examples:
//   gasman_run scenario.xml --end 600 --every 5
//   gasman_run scenario.csv --output results.csv
//   gasman_run scenario.xlsx --output results.csv
//   gasman_run data.json --start 0 --end 300 --every 10 --output out.csv

#include "XmlToJson.h"          // xmlStringToJson()
#include "XlsxReader.h"         // parseXlsxFile()
#include "../convert/CsvReader.h"    // parseCsvFile(), GasManInput
#include "../convert/GasManWriter.h" // toJson()

// GasMan simulation engine (linked statically — no DLL needed at runtime)
#include "GasApplication.h"

#include <json.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <cstdlib>
#include <locale>
#include <stdexcept>

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static std::string readFile(const std::string& path)
{
    std::ifstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("Cannot open file: " + path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static std::string lowerExt(const std::string& path)
{
    std::string ext = std::filesystem::path(path).extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return ext;
}

static void printUsage(const char* prog)
{
    std::cerr
        << "Usage: " << prog << " <input.[xml|json|csv|xlsx]> [options]\n"
        << "\n"
        << "Options:\n"
        << "  --start  <sec>   Start time in seconds  (default 0)\n"
        << "  --end    <sec>   End time in seconds    (default: last setting time)\n"
        << "  --every  <sec>   Row interval in secs   (default 10)\n"
        << "  --output <file>  Write CSV to <file>    (default: stdout)\n"
        << "  --ini    <file>  Path to gasman.ini     (default: auto-locate)\n"
        << "\n"
        << "Supported input formats:\n"
        << "  .xml   GasMan XML scenario\n"
        << "  .json  GasMan JSON scenario\n"
        << "  .csv   Sections-based CSV  (see examples/simulation_template.csv)\n"
        << "  .xlsx  Excel workbook      (see examples/gasman_template.xlsx)\n"
        << "  .gas   Not supported — open in GasMan and export as XML or JSON\n"
        << "\n"
        << "Examples:\n"
        << "  " << prog << " scenario.xml --end 600 --every 5\n"
        << "  " << prog << " scenario.csv --output results.csv\n"
        << "  " << prog << " scenario.xlsx --output results.csv\n"
        << "  " << prog << " data.json --start 0 --end 300 --every 10\n";
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    if (argc < 2) { printUsage(argv[0]); return 1; }

    // ── Parse arguments ──────────────────────────────────────────────────────
    std::string inputPath;
    std::string outputPath;
    std::string iniPath;
    int startSec  = 0;
    int endSec    = 300;   // used as fallback when auto-detection finds nothing
    int everySec  = 10;
    bool endAuto  = true;  // auto-detect end time from last setting

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        auto need = [&](const char* flag) -> std::string {
            if (i + 1 >= argc) {
                std::cerr << "Error: " << flag << " requires a value\n";
                std::exit(1);
            }
            return argv[++i];
        };

        if      (a == "--start")  startSec   = std::atoi(need("--start").c_str());
        else if (a == "--end")  { endSec = std::atoi(need("--end").c_str()); endAuto = false; }
        else if (a == "--every")  everySec   = std::atoi(need("--every").c_str());
        else if (a == "--output") outputPath = need("--output");
        else if (a == "--ini")    iniPath    = need("--ini");
        else if (a == "--help" || a == "-h") { printUsage(argv[0]); return 0; }
        else if (!a.empty() && a[0] == '-') {
            std::cerr << "Error: unknown option '" << a << "'\n";
            printUsage(argv[0]);
            return 1;
        } else {
            if (!inputPath.empty()) {
                std::cerr << "Error: unexpected argument '" << a << "'\n";
                return 1;
            }
            inputPath = a;
        }
    }

    if (inputPath.empty()) {
        std::cerr << "Error: no input file specified\n";
        printUsage(argv[0]);
        return 1;
    }

    // ── Reject unsupported formats ────────────────────────────────────────────
    {
        std::string ext = lowerExt(inputPath);

        if (ext == ".gas") {
            std::cerr
                << "Error: .gas files are GasMan's Qt binary format and cannot be\n"
                << "  read directly by gasman_run.\n"
                << "\n"
                << "  To convert a .gas file:\n"
                << "    1. Open it in the GasMan desktop application.\n"
                << "    2. Use File > Save As and choose XML or JSON.\n"
                << "    3. Then run:\n"
                << "         gasman_run scenario.xml\n"
                << "      or gasman_run scenario.json\n";
            return 1;
        }

        if (ext == ".xls") {
            std::cerr
                << "Error: Legacy .xls (Excel 97-2003) files are not supported.\n"
                << "\n"
                << "  Open the file in Excel and save it as .xlsx, then run:\n"
                << "    gasman_run scenario.xlsx\n";
            return 1;
        }
    }

    // ── Convert input → JSON string ──────────────────────────────────────────
    std::string jsonStr;
    try {
        std::string ext = lowerExt(inputPath);

        if (ext == ".xml") {
            std::string xmlStr = readFile(inputPath);
            nlohmann::json j   = xmlStringToJson(xmlStr);
            jsonStr            = j.dump();
        }
        else if (ext == ".json") {
            jsonStr = readFile(inputPath);
        }
        else if (ext == ".csv") {
            GasManInput inp = parseCsvFile(inputPath);
            jsonStr         = toJson(inp);
        }
        else if (ext == ".xlsx") {
            GasManInput inp = parseXlsxFile(inputPath);
            jsonStr         = toJson(inp);
        }
        else {
            std::cerr << "Error: unrecognised file extension '" << ext
                      << "'.  Use .xml, .json, .csv, or .xlsx\n";
            return 1;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error reading input: " << ex.what() << "\n";
        return 1;
    }

    // ── Auto-detect end time from last setting ───────────────────────────────
    if (endAuto) {
        try {
            auto j = nlohmann::json::parse(jsonStr);
            const auto& settings = j.value("settings", nlohmann::json::array());
            if (settings.is_array() && !settings.empty()) {
                std::string tStr = settings.back().value("time", std::string("00:00:00"));
                std::istringstream ts(tStr);
                std::string tok;
                int parts[3] = {0, 0, 0};
                int idx = 0;
                while (idx < 3 && std::getline(ts, tok, ':'))
                    parts[idx++] = std::stoi(tok);
                int computed = parts[0] * 3600 + parts[1] * 60 + parts[2];
                endSec = std::max(endSec, computed);
            }
        } catch (...) {}
    }

    // ── Initialise the simulation engine ─────────────────────────────────────
    GasApplication app;

    // Override INI path if provided on the command line.
    // (GasApplication locates gasman.ini automatically relative to the exe,
    //  but the user can override with --ini if needed.)
    if (!iniPath.empty()) {
        // GasApplication exposes getIniFile(); to override we rely on the
        // working-directory fallback built into GasApplication — so just set
        // the working directory to the folder containing the INI.
        std::filesystem::path iniAbsolute = std::filesystem::absolute(iniPath);
        if (!std::filesystem::exists(iniAbsolute)) {
            std::cerr << "Error: gasman.ini not found at '" << iniAbsolute.string() << "'\n";
            return 1;
        }
        // Canonicalise so the app can find it.
        iniPath = iniAbsolute.string();
    }

    // Check gasman.ini is reachable.
    {
        std::ifstream iniCheck(app.getIniFile());
        if (!iniCheck.good()) {
            std::cerr
                << "Error: gasman.ini not found (looked in '" << app.getIniFile() << "').\n"
                << "  Copy gasman.ini next to this executable, or use --ini <path>.\n";
            return 1;
        }
    }

    // Set locale from INI (matches the behaviour in dllmain.cpp).
    {
        std::string locale = std::locale("").name();
        app.ReadProfile("Application", "Locale", locale);
        try {
            std::locale::global(std::locale(locale));
        } catch (...) {
            std::locale::global(std::locale("en_US.UTF-8"));
        }
    }

    if (!app.initialize()) {
        std::cerr << "Error: GasApplication::initialize() failed — check gasman.ini\n";
        return 1;
    }

    // ── Run simulation ───────────────────────────────────────────────────────
    std::string csvResult;
    try {
        csvResult = GasApplication::createCSV(
            jsonStr.c_str(),
            static_cast<int>(jsonStr.size()),
            startSec, endSec, everySec);
    }
    catch (const std::exception& ex) {
        std::cerr << "Error during simulation: " << ex.what() << "\n";
        return 1;
    }

    if (csvResult.empty() || csvResult.rfind("Error", 0) == 0) {
        std::cerr << "Simulation error: " << csvResult << "\n";
        return 1;
    }

    // ── Write output ─────────────────────────────────────────────────────────
    if (outputPath.empty()) {
        std::cout << csvResult;
    } else {
        std::ofstream out(outputPath);
        if (!out) {
            std::cerr << "Error: cannot open output file '" << outputPath << "'\n";
            return 1;
        }
        out << csvResult;
        std::cerr << "Written to " << outputPath << "\n";
    }

    return 0;
}
