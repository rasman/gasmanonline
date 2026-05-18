#pragma once
// XmlToJson.h — minimal GasMan XML → nlohmann::json converter.
//
// Replicates the Python xml_to_dict() semantics used by python_gas.py:
//   • Each element becomes a JSON object with a "tagName" key.
//   • XML attributes become string-valued keys in that object.
//   • Child elements become nested objects under the child's tag name.
//   • If multiple siblings share the same tag they collapse into a JSON array.
//
// No external XML library is required.  The parser handles the subset of XML
// used by GasMan files: elements, attributes (single or double quoted),
// XML/stylesheet PIs, comments, and the five predefined entities.
// CDATA sections and namespace prefixes are not needed by GasMan.

#include <json.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <cctype>
#include <optional>

// ---------------------------------------------------------------------------
// Internal DOM node (intermediate representation)
// ---------------------------------------------------------------------------
namespace xml_detail {

struct Node {
    std::string tag;
    std::unordered_map<std::string, std::string> attrs;
    std::vector<Node> children;
};

// ---------------------------------------------------------------------------
// Character-level helpers
// ---------------------------------------------------------------------------

inline bool isNameChar(char c)
{
    return std::isalnum(static_cast<unsigned char>(c))
        || c == '_' || c == '-' || c == '.' || c == ':';
}

inline std::string unescapeXml(const std::string& s)
{
    std::string out;
    out.reserve(s.size());
    size_t i = 0;
    while (i < s.size()) {
        if (s[i] == '&') {
            size_t semi = s.find(';', i + 1);
            if (semi != std::string::npos) {
                std::string ent = s.substr(i + 1, semi - i - 1);
                if      (ent == "amp")  out += '&';
                else if (ent == "lt")   out += '<';
                else if (ent == "gt")   out += '>';
                else if (ent == "quot") out += '"';
                else if (ent == "apos") out += '\'';
                else                    out += s.substr(i, semi - i + 1);
                i = semi + 1;
                continue;
            }
        }
        out += s[i++];
    }
    return out;
}

// ---------------------------------------------------------------------------
// Parser
// ---------------------------------------------------------------------------

class Parser {
    const std::string& src;
    size_t             pos;

public:
    explicit Parser(const std::string& s) : src(s), pos(0) {}

    // Entry point: return the first real element found in the document.
    std::optional<Node> parseDocument()
    {
        while (pos < src.size()) {
            skipToTag();
            if (pos >= src.size()) break;
            // pos == '<'
            auto node = readNode();
            if (node.has_value())
                return node;   // found the root element
        }
        return std::nullopt;
    }

private:
    // ── low-level character helpers ─────────────────────────────────────────

    void skipWhitespace()
    {
        while (pos < src.size() && std::isspace(static_cast<unsigned char>(src[pos])))
            ++pos;
    }

    // Advance pos until we see '<' (or end of string).
    void skipToTag()
    {
        while (pos < src.size() && src[pos] != '<')
            ++pos;
    }

    // Advance past the next occurrence of 'end'.
    void skipPast(const std::string& end)
    {
        size_t found = src.find(end, pos);
        pos = (found != std::string::npos) ? found + end.size() : src.size();
    }

    // Advance pos past the next '>'.
    void skipToClosingAngle()
    {
        while (pos < src.size() && src[pos] != '>')
            ++pos;
        if (pos < src.size()) ++pos;  // consume '>'
    }

    std::string readName()
    {
        size_t start = pos;
        while (pos < src.size() && isNameChar(src[pos]))
            ++pos;
        return src.substr(start, pos - start);
    }

    // Read a quoted attribute value; pos must be on the opening ' or ".
    std::string readAttrValue()
    {
        if (pos >= src.size()) return {};
        char q = src[pos++];          // opening quote
        size_t start = pos;
        while (pos < src.size() && src[pos] != q)
            ++pos;
        std::string val = src.substr(start, pos - start);
        if (pos < src.size()) ++pos;  // closing quote
        return unescapeXml(val);
    }

    // ── node reader ─────────────────────────────────────────────────────────

    // Called with pos pointing AT '<'.  Returns the parsed element, or
    // std::nullopt for PIs, comments, and closing tags.
    std::optional<Node> readNode()
    {
        if (pos >= src.size() || src[pos] != '<')
            return std::nullopt;
        ++pos;  // consume '<'

        if (pos >= src.size()) return std::nullopt;

        // Processing instruction  <?...?>
        if (src[pos] == '?') { skipPast("?>"); return std::nullopt; }

        // Comment or DOCTYPE  <!--...--> / <!...>
        if (src[pos] == '!') { skipToClosingAngle(); return std::nullopt; }

        // Closing tag  </tag>
        if (src[pos] == '/') { skipToClosingAngle(); return std::nullopt; }

        // ── Opening / self-closing tag ───────────────────────────────────────
        Node node;
        node.tag = readName();
        if (node.tag.empty()) { skipToClosingAngle(); return std::nullopt; }

        // Read attributes
        while (pos < src.size()) {
            skipWhitespace();
            if (src[pos] == '>' || src[pos] == '/') break;

            std::string name = readName();
            if (name.empty()) { ++pos; continue; }  // skip unexpected char

            skipWhitespace();
            if (pos < src.size() && src[pos] == '=') {
                ++pos;  // consume '='
                skipWhitespace();
                node.attrs[name] = readAttrValue();
            } else {
                node.attrs[name] = "";  // boolean attribute
            }
        }

        // Self-closing  <tag ... />
        if (pos < src.size() && src[pos] == '/') {
            ++pos;
            if (pos < src.size() && src[pos] == '>') ++pos;
            return node;
        }
        // Consume '>'
        if (pos < src.size() && src[pos] == '>') ++pos;

        // ── Parse children until the matching closing tag ────────────────────
        while (pos < src.size()) {
            skipToTag();
            if (pos >= src.size()) break;

            // Is the next tag a closing tag?
            if (pos + 1 < src.size() && src[pos + 1] == '/') {
                // Consume </tag>
                skipToClosingAngle();
                break;
            }

            auto child = readNode();
            if (child.has_value())
                node.children.push_back(std::move(*child));
        }

        return node;
    }
};

// ---------------------------------------------------------------------------
// Convert the intermediate DOM to nlohmann::json (xml_to_dict semantics)
// ---------------------------------------------------------------------------

inline nlohmann::json nodeToJson(const Node& node)
{
    nlohmann::json j;
    j["tagName"] = node.tag;

    for (auto& [k, v] : node.attrs)
        j[k] = v;

    for (auto& child : node.children) {
        nlohmann::json childJson = nodeToJson(child);
        if (!j.contains(child.tag)) {
            j[child.tag] = childJson;
        } else {
            // Promote to array on second occurrence of the same tag.
            if (!j[child.tag].is_array()) {
                nlohmann::json existing = j[child.tag];
                j[child.tag] = nlohmann::json::array();
                j[child.tag].push_back(std::move(existing));
            }
            j[child.tag].push_back(std::move(childJson));
        }
    }
    return j;
}

} // namespace xml_detail

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

/// Parse a GasMan XML string and return it as a nlohmann::json object that
/// matches the structure produced by python_gas.py's xml_to_dict().
/// Throws std::runtime_error if no root element is found.
inline nlohmann::json xmlStringToJson(const std::string& xmlStr)
{
    xml_detail::Parser p(xmlStr);
    auto root = p.parseDocument();
    if (!root.has_value())
        throw std::runtime_error("XmlToJson: no root element found in XML");
    return xml_detail::nodeToJson(*root);
}
