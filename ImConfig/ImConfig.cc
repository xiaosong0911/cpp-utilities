#include "ImConfig.h"
#include "../bc/bc.h"
#include "../string_helper.h"

namespace ImConfig {

static std::string readXMLElement(const XMLElement * elem) {
    if (!elem) return "";
    std::string str = elem->GetText() ? elem->GetText() : "";
    // substitute {} quotation
    for (;;) {
        size_t start = str.find('{');
        size_t end = str.find('}');
        if (start == std::string::npos) break;
        if (end == std::string::npos) break;

        const std::vector<std::string> path = string_helper::split(str.substr(start + 1, end - start - 1), "/");
        const XMLNode * cursor = elem->Parent();
        if (path[0] == "") cursor = elem->GetDocument(); // root
        for (const std::string & n : path) {
            if (!cursor) break;
            if (n == "." || n.empty()) {}
            else if (n == "..")
                cursor = cursor->Parent();
            else
                cursor = cursor->FirstChildElement(n.c_str());
        }
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


void ImConfig::loadXML(XMLElement * elem) {
    if (!elem) return;
    for (XMLElement * term = elem->FirstChildElement();
            term;
            term = term->NextSiblingElement()) {
        map[term->Name()] = readXMLElement(term);
    }
}

void ImConfig::saveXML(XMLElement * elem) {
    if (!elem) return;
    for (auto p : map) {
        XMLElement * term = elem->FirstChildElement(p.first.c_str());
        if (!term) {
            term = elem->GetDocument()->NewElement(p.first.c_str());
            term->SetText(p.second.c_str());
            elem->InsertEndChild(term);
        }
    }
}

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

}
