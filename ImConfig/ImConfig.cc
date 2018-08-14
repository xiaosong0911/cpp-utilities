#include "ImConfig.h"
#include <tinyxml2.h>
#include "../bc/bc.h"

namespace ImConfig {

using namespace tinyxml2;

void ImConfig::loadFile(const char * filename) {
    XMLDocument doc;
    doc.LoadFile(filename);
    XMLElement * config = doc.FirstChildElement("config");
    if (!config) return;
    for (XMLElement * term = config->FirstChildElement();
            term;
            term = term->NextSiblingElement()) {
        map[term->Name()] = term->GetText() ?  term->GetText() : "";
    }
}

void ImConfig::saveFile(const char * filename) {
    XMLDocument doc;
    doc.LoadFile(filename);
    XMLElement * config = doc.FirstChildElement("config");
    if (!config) {
        config = doc.NewElement("config");
        doc.InsertEndChild(config);
    }
    for (auto p : map) {
        XMLElement * term = config->FirstChildElement(p.first.c_str());
        if (!term) {
            term = doc.NewElement(p.first.c_str());
            term->SetText(p.second.c_str());
            config->InsertEndChild(term);
        }
    }
    doc.SaveFile(filename);
}

const char * ImConfig::getText(const char * name) {
    if (map.count(name))
        return map[name].c_str();
    else
        return "";
}

void ImConfig::setText(const char * name, const char * text) {
    map[name] = std::string(text);
}

std::string ImConfig::eval(std::string str) {
    // substitute {} quotation
    for (;;) {
        size_t start = str.find('{');
        size_t end = str.find('}');
        if (start == std::string::npos) break;
        if (end == std::string::npos) break;

        std::string tag = str.substr(start + 1, end - start - 1);
        str.replace(start, end + 1 - start, eval(getText(tag.c_str())));
    }
    // evaluate [] quotation
    for (;;) {
        size_t start = str.find('[');
        size_t end = str.find(']');
        if (start == std::string::npos) break;
        if (end == std::string::npos) break;

        std::string expr = str.substr(start + 1, end - start - 1);
        double r;
        char buffer[64] = "";
        if (bc::eval(expr.c_str(), r)) {
            sprintf(buffer, "%lg", r);
        }
        str.replace(start, end + 1 - start, buffer);
    }
    return str;
}
}
