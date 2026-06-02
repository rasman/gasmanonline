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
    // Emit a numeric agent field ONLY when it was actually supplied (> 0).  An
    // unset field defaults to 0 in GasManInput; emitting that 0 would overwrite
    // the engine's gasman.ini value (e.g. lambdaBlood = 0 → divide-by-zero →
    // NaN).  Omitting it lets the agent's ini section win — so "[agent] name,X"
    // alone is a valid, fully ini-driven agent.
    json agentsArr = json::array();
    for (const auto& a : inp.agents) {
        json agent = { {"name", a.name} };
        auto put = [&](const char* k, float v) {
            if (v > 0.0f) agent[k] = std::stod(fmtF(v));
        };
        put("lambdaBlood", a.lambdaBlood);
        put("lambdaVrg",   a.lambdaVrg);
        put("lambdaFat",   a.lambdaFat);
        put("lambdaMus",   a.lambdaMus);
        put("volatility",  a.volatility);
        put("bottleSize",  a.bottleSize);
        put("bottleCost",  a.bottleCost);
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

    // ── Params: include only blocks the input actually supplied ───────────────
    // Omitted blocks fall back to gasman.ini in the engine (the ini wins).
    json patient = json::object();
    if (inp.hasWeight)
        patient["weight"] = {{"value", std::stod(fmtF(inp.weightKg))}, {"unit", "kilograms"}};
    if (inp.hasVolumes)
        patient["volumes"] = {
            {"vrg", std::stod(fmtF(inp.vrgVol))},
            {"fat", std::stod(fmtF(inp.fatVol))},
            {"ven", std::stod(fmtF(inp.venVol))},
            {"alv", std::stod(fmtF(inp.alvVol))},
            {"mus", std::stod(fmtF(inp.musVol))}
        };
    if (inp.hasFlows)
        patient["flows"] = {
            {"vrg", std::stod(fmtF(inp.vrgFlow))},
            {"fat", std::stod(fmtF(inp.fatFlow))},
            {"mus", std::stod(fmtF(inp.musFlow))}
        };

    json params = { {"agents", agentsArr} };
    if (!patient.empty())
        params["patient"] = patient;
    if (inp.hasDt)
        params["dt_ms"] = std::stod(fmtF(inp.dtMs));

    // ── Root document ─────────────────────────────────────────────────────────
    json root = {
        {"description", ""},
        {"params", params},
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

    // <params> — emit only the patient blocks the input supplied; omitted ones
    // fall back to gasman.ini in the engine (the ini wins).
    x << "  <params>\n";
    if (inp.hasWeight || inp.hasVolumes || inp.hasFlows) {
        x << "    <patient>\n";
        if (inp.hasWeight)
            x << "      <weight"
              << attr("unit",  std::string("kilograms"))
              << attr("value", inp.weightKg)
              << "/>\n";
        if (inp.hasVolumes)
            x << "      <volumes"
              << attr("vrg", inp.vrgVol)
              << attr("fat", inp.fatVol)
              << attr("ven", inp.venVol)
              << attr("alv", inp.alvVol)
              << attr("mus", inp.musVol)
              << "/>\n";
        if (inp.hasFlows)
            x << "      <flows"
              << attr("vrg", inp.vrgFlow)
              << attr("fat", inp.fatFlow)
              << attr("mus", inp.musFlow)
              << "/>\n";
        x << "    </patient>\n";
    }

    // Emit an agent attribute only when supplied (> 0); an unset 0 would
    // overwrite the engine's gasman.ini value and break the model (NaN).
    x << "    <agents>\n";
    for (const auto& a : inp.agents) {
        x << "      <agent" << attr("name", a.name);
        if (a.lambdaMus   > 0.0f) x << attr("lambdaMus",   a.lambdaMus);
        if (a.lambdaBlood > 0.0f) x << attr("lambdaBlood", a.lambdaBlood);
        if (a.lambdaFat   > 0.0f) x << attr("lambdaFat",   a.lambdaFat);
        if (a.volatility  > 0.0f) x << attr("volatility",  a.volatility);
        if (a.bottleSize  > 0.0f) x << attr("bottleSize",  a.bottleSize);
        if (a.lambdaVrg   > 0.0f) x << attr("lambdaVrg",   a.lambdaVrg);
        if (a.bottleCost  > 0.0f) x << attr("bottleCost",  a.bottleCost);
        x << "/>\n";
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
