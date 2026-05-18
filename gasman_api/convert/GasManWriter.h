#pragma once
// GasManWriter.h — header-only output writer for GasMan JSON and XML formats.
//
// Provides:
//   std::string toJson(const GasManInput&)
//   std::string toXml (const GasManInput&)
//
// JSON uses the GasMan native format (numeric values, plain arrays) that the
// core desktop app produces under File → Export → JSON.  This is also the
// format accepted by GasManJsonToCsv().
//
// XML uses the GasMan native XML schema exported by the desktop app.
//
// Both formats support an arbitrary number of agents.  Delivery columns in the
// settings are matched to agents positionally via GasManInput::agentSettings.
//
// nlohmann/json.hpp is available via the gasmanAPI/importLibraries include path.

#include "CsvReader.h"          // GasManInput, AgentParams, AgentSetting, Setting
#include <json.hpp>             // nlohmann/json
#include <string>
#include <sstream>
#include <iomanip>

// ---------------------------------------------------------------------------
// Shared float formatter
// ---------------------------------------------------------------------------
namespace writer_detail {

/// Format a float as a compact decimal string (up to 6 significant digits,
/// no trailing zeros, no scientific notation for normal clinical values).
inline std::string fmtF(float v)
{
    std::ostringstream oss;
    oss << std::setprecision(6) << std::defaultfloat << v;
    return oss.str();
}

} // namespace writer_detail

// ---------------------------------------------------------------------------
// JSON output  (GasMan native format)
// ---------------------------------------------------------------------------

inline std::string toJson(const GasManInput& inp)
{
    using json = nlohmann::json;
    using writer_detail::fmtF;

    // ── Agents array ─────────────────────────────────────────────────────────
    json agentsArr = json::array();
    for (const auto& a : inp.agents) {
        json agent = {
            {"name",        a.name},
            {"lambdaBlood", std::stod(fmtF(a.lambdaBlood))},
            {"lambdaVrg",   std::stod(fmtF(a.lambdaVrg))},
            {"lambdaFat",   std::stod(fmtF(a.lambdaFat))},
            {"lambdaMus",   std::stod(fmtF(a.lambdaMus))},
            {"volatility",  std::stod(fmtF(a.volatility))},
            {"bottleSize",  std::stod(fmtF(a.bottleSize))},
            {"bottleCost",  std::stod(fmtF(a.bottleCost))}
        };
        agentsArr.push_back(agent);
    }

    // ── Settings array ────────────────────────────────────────────────────────
    json settingsArr = json::array();
    for (const auto& s : inp.settings) {
        // One agent_settings entry per agent.
        json agentSettingsArr = json::array();
        for (size_t i = 0; i < inp.agents.size(); ++i) {
            float del    = (i < s.agentSettings.size()) ? s.agentSettings[i].del    : 0.0f;
            float inject = (i < s.agentSettings.size()) ? s.agentSettings[i].inject : 0.0f;
            json as = {
                {"name",   inp.agents[i].name},
                {"del",    std::stod(fmtF(del))},
                {"inject", std::stod(fmtF(inject))}
            };
            agentSettingsArr.push_back(as);
        }

        json setting = {
            {"time",           s.time},
            {"va",             std::stod(fmtF(s.va))},
            {"fgf",            std::stod(fmtF(s.fgf))},
            {"co",             std::stod(fmtF(s.co))},
            {"circuit",        s.circuit},
            {"agent_settings", agentSettingsArr}
        };
        settingsArr.push_back(setting);
    }

    // ── Root document ─────────────────────────────────────────────────────────
    json root = {
        {"description", ""},
        {"params", {
            {"patient", {
                {"weight",  {{"value", std::stod(fmtF(inp.weightKg))}, {"unit", "kilograms"}}},
                {"volumes", {
                    {"vrg", std::stod(fmtF(inp.vrgVol))},
                    {"fat", std::stod(fmtF(inp.fatVol))},
                    {"ven", std::stod(fmtF(inp.venVol))},
                    {"alv", std::stod(fmtF(inp.alvVol))},
                    {"mus", std::stod(fmtF(inp.musVol))}
                }},
                {"flows", {
                    {"vrg", std::stod(fmtF(inp.vrgFlow))},
                    {"fat", std::stod(fmtF(inp.fatFlow))},
                    {"mus", std::stod(fmtF(inp.musFlow))}
                }}
            }},
            {"agents", agentsArr}
        }},
        {"settings", settingsArr}
    };

    return root.dump(2);
}

// ---------------------------------------------------------------------------
// XML helpers
// ---------------------------------------------------------------------------
namespace xml_detail {

/// Escape the five predefined XML entities in an attribute value or text node.
inline std::string escapeXml(const std::string& s)
{
    std::string out;
    out.reserve(s.size());
    for (unsigned char c : s) {
        switch (c) {
        case '&':  out += "&amp;";  break;
        case '<':  out += "&lt;";   break;
        case '>':  out += "&gt;";   break;
        case '"':  out += "&quot;"; break;
        case '\'': out += "&apos;"; break;
        default:   out += c;        break;
        }
    }
    return out;
}

/// Format a float as a compact decimal string.
inline std::string fmtF(float v)
{
    return writer_detail::fmtF(v);
}

/// attr="value" pair for a float.
inline std::string attr(const std::string& name, float v)
{
    return " " + name + "=\"" + fmtF(v) + "\"";
}

/// attr="value" pair for a string (XML-escaped).
inline std::string attr(const std::string& name, const std::string& v)
{
    return " " + name + "=\"" + escapeXml(v) + "\"";
}

} // namespace xml_detail

// ---------------------------------------------------------------------------
// XML output
// ---------------------------------------------------------------------------

inline std::string toXml(const GasManInput& inp)
{
    using namespace xml_detail;

    std::ostringstream x;

    x << "<?xml version='1.0' encoding='utf-8'?>\n";
    x << "<gasman description=\"\">\n";

    // <params>
    x << "  <params>\n";
    x << "    <patient>\n";
    x << "      <weight"
      << attr("unit",  std::string("kilograms"))
      << attr("value", inp.weightKg)
      << "/>\n";
    x << "      <volumes"
      << attr("vrg", inp.vrgVol)
      << attr("fat", inp.fatVol)
      << attr("ven", inp.venVol)
      << attr("alv", inp.alvVol)
      << attr("mus", inp.musVol)
      << "/>\n";
    x << "      <flows"
      << attr("vrg", inp.vrgFlow)
      << attr("fat", inp.fatFlow)
      << attr("mus", inp.musFlow)
      << "/>\n";
    x << "    </patient>\n";

    x << "    <agents>\n";
    for (const auto& a : inp.agents) {
        x << "      <agent"
          << attr("name",        a.name)
          << attr("lambdaMus",   a.lambdaMus)
          << attr("lambdaBlood", a.lambdaBlood)
          << attr("lambdaFat",   a.lambdaFat)
          << attr("volatility",  a.volatility)
          << attr("bottleSize",  a.bottleSize)
          << attr("lambdaVrg",   a.lambdaVrg)
          << attr("bottleCost",  a.bottleCost)
          << "/>\n";
    }
    x << "    </agents>\n";
    x << "  </params>\n";

    // <settings>
    x << "  <settings>\n";
    for (const auto& s : inp.settings) {
        x << "    <setting"
          << attr("va",      s.va)
          << attr("fgf",     s.fgf)
          << attr("circuit", s.circuit)
          << attr("time",    s.time)
          << attr("co",      s.co)
          << ">\n";
        x << "      <agentsettings>\n";
        for (size_t i = 0; i < inp.agents.size(); ++i) {
            float del    = (i < s.agentSettings.size()) ? s.agentSettings[i].del    : 0.0f;
            float inject = (i < s.agentSettings.size()) ? s.agentSettings[i].inject : 0.0f;
            x << "        <agentsetting"
              << attr("del",       del)
              << attr("inject",    inject)
              << attr("agentname", inp.agents[i].name)
              << "/>\n";
        }
        x << "      </agentsettings>\n";
        x << "    </setting>\n";
    }
    x << "  </settings>\n";

    x << "</gasman>\n";

    return x.str();
}
