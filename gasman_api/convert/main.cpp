// gasman_convert — Convert a GasMan sections-based CSV file to JSON or XML.
//
// Usage:
//   gasman_convert <input.csv> [--format xml|json] [--output <file>]
//
// Options:
//   --format json   Output GasMan JSON  (default)
//   --format xml    Output GasMan XML
//   --output <file> Write to <file> instead of stdout
//
// Notes:
//   - Lines starting with # are treated as comments.
//   - .xlsx files are NOT supported directly.  Export the sheet as CSV first,
//     or use python_gas.py which handles Excel natively.

#include "CsvReader.h"
#include "GasManWriter.h"

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <stdexcept>

static void printUsage(const char* progname)
{
    std::cerr
        << "Usage: " << progname
        << " <input.csv> [--format xml|json] [--output <file>]\n"
        << "\n"
        << "  --format json   Produce GasMan JSON (default)\n"
        << "  --format xml    Produce GasMan XML\n"
        << "  --output <file> Write output to <file> (default: stdout)\n"
        << "\n"
        << "Example:\n"
        << "  gasman_convert simulation.csv --format json --output data.json\n"
        << "  gasman_convert simulation.csv --format xml\n";
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string inputPath;
    std::string format     = "json";   // default output format
    std::string outputPath;            // empty → stdout

    // Simple argument parsing (no external library).
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--format") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --format requires an argument (json or xml)\n";
                return 1;
            }
            format = argv[++i];
            // lower-case
            std::transform(format.begin(), format.end(), format.begin(),
                           [](unsigned char c){ return std::tolower(c); });
            if (format != "json" && format != "xml") {
                std::cerr << "Error: unknown format '" << format
                          << "'.  Use 'json' or 'xml'.\n";
                return 1;
            }
        } else if (arg == "--output") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --output requires a file path argument\n";
                return 1;
            }
            outputPath = argv[++i];
        } else if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        } else if (!arg.empty() && arg[0] == '-') {
            std::cerr << "Error: unknown option '" << arg << "'\n";
            printUsage(argv[0]);
            return 1;
        } else {
            // Positional argument: the input file.
            if (!inputPath.empty()) {
                std::cerr << "Error: unexpected argument '" << arg << "'\n";
                printUsage(argv[0]);
                return 1;
            }
            inputPath = arg;
        }
    }

    if (inputPath.empty()) {
        std::cerr << "Error: no input file specified\n";
        printUsage(argv[0]);
        return 1;
    }

    // ---------------------------------------------------------------------------
    // Reject .xlsx files with a helpful message.
    // ---------------------------------------------------------------------------
    {
        std::filesystem::path p(inputPath);
        std::string ext = p.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        if (ext == ".xlsx" || ext == ".xls") {
            std::cerr
                << "Error: Excel files (.xlsx / .xls) are not supported directly.\n"
                << "\n"
                << "  Option 1 — use python_gas.py (handles Excel natively):\n"
                << "    python python_gas.py " << inputPath << "\n"
                << "\n"
                << "  Option 2 — save the worksheet as CSV first, then run:\n"
                << "    gasman_convert simulation.csv\n";
            return 1;
        }
    }

    // ---------------------------------------------------------------------------
    // Parse the CSV.
    // ---------------------------------------------------------------------------
    GasManInput inp;
    try {
        inp = parseCsvFile(inputPath);
    } catch (const std::exception& ex) {
        std::cerr << "Error reading '" << inputPath << "': " << ex.what() << "\n";
        return 1;
    }

    // ---------------------------------------------------------------------------
    // Generate output.
    // ---------------------------------------------------------------------------
    std::string output;
    try {
        if (format == "xml")
            output = toXml(inp);
        else
            output = toJson(inp);
    } catch (const std::exception& ex) {
        std::cerr << "Error generating output: " << ex.what() << "\n";
        return 1;
    }

    // ---------------------------------------------------------------------------
    // Write output.
    // ---------------------------------------------------------------------------
    if (outputPath.empty()) {
        std::cout << output;
    } else {
        std::ofstream outFile(outputPath);
        if (!outFile) {
            std::cerr << "Error: Cannot open output file '" << outputPath << "'\n";
            return 1;
        }
        outFile << output;
        if (!outFile) {
            std::cerr << "Error: Failed while writing to '" << outputPath << "'\n";
            return 1;
        }
        std::cerr << "Written to " << outputPath << "\n";
    }

    return 0;
}
