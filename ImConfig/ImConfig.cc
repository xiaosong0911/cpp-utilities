#include "ImConfig.h"
#include "../bc/bc.h"
#include "../string_helper.h"

namespace ImConfig {

const char * ImConfig::getText(const char * name) {
    if (map.count(name))
        return map[name].c_str();
    else
        return nullptr;
}

void ImConfig::setText(const char * name, const char * text) {
    map[name] = std::string(text);
}

void ImConfig::clear(const char * name) {
    auto it = map.find(name);
    if (it != map.end()) map.erase(it);
}

static const XMLNode * xmlPathFrom(const XMLNode * cursor, const std::string & pstr) {
    if (!cursor || pstr.empty()) return cursor;
    const std::vector<std::string> path = string_helper::split(pstr, "/");
    if (path[0] == "") cursor = cursor->GetDocument(); // root
    for (const std::string & n : path) {
        if (!cursor) break;
        if (n == "." || n.empty()) {}
        else if (n == "..")
            cursor = cursor->Parent();
        else
            cursor = cursor->FirstChildElement(n.c_str());
    }
    return cursor;
}

static std::string readXMLElement(const XMLElement * elem) {
    if (!elem) return "";
    std::string str = elem->GetText() ? elem->GetText() : "";
    // substitute {} quotation
    for (;;) {
        size_t start = str.find('{');
        size_t end = str.find('}');
        if (start == std::string::npos) break;
        if (end == std::string::npos) break;

        const XMLNode * cursor = xmlPathFrom(elem->Parent(), str.substr(start + 1, end - start - 1));
        if (cursor)
            str.replace(start, end + 1 - start, readXMLElement(cursor->ToElement()));
        else
            str.replace(start, end + 1 - start, "");
    }
    // evaluate [] arithemic operations
    for (;;) {
        size_t start = str.find('[');
        size_t end = str.find(']');
        if (start == std::string::npos) break;
        if (end == std::string::npos) break;

        std::string expr = str.substr(start + 1, end - start - 1);
        double r;
        std::string buffer;
        if (bc::eval(expr.c_str(), r)) {
            string_helper::sprintf(buffer, "%lg", r);
        }
        str.replace(start, end + 1 - start, buffer);
    }
    return str;
}

void ImConfigFactory::loadXML(XMLElement * elem, ImConfig & ic) {
    if (!elem) return;
    std::map<std::string, std::string> map;
    for (XMLElement * term = elem->FirstChildElement();
            term;
            term = term->NextSiblingElement()) {
        map[term->Name()] = readXMLElement(term);
    }
    ic.modMap(map);
}

void ImConfigFactory::saveXML(XMLElement * elem, const ImConfig & ic, bool overwrite) {
    if (!elem) return;
    std::map<std::string, std::string> map = ic.getMap();
    for (auto p : map) {
        XMLElement * term = elem->FirstChildElement(p.first.c_str());
        if (!term) {
            term = elem->GetDocument()->NewElement(p.first.c_str());
            if (!overwrite) term->SetText(p.second.c_str());
            elem->InsertEndChild(term);
        }
        if (overwrite) term->SetText(p.second.c_str());
    }
}

XMLElement * ImConfigFactory::getElement(const char * tag) {
    XMLElement * e = doc.FirstChildElement(tag);
    if (!e) {
        e = doc.NewElement(tag);
        doc.InsertEndChild(e);
    }
    return e;
}

ImConfig ImConfigFactory::createImConfig(const char * tag, bool overwrite) {
    XMLElement * e = getElement(tag);
    ImConfig ic;
    loadXML(e, ic);
    ic.setCB([=](const ImConfig &ic) {
        saveXML(e, ic, overwrite);
    });
    return ic;
}

}
