#include "ImConfig.h"

namespace ImConfig {

void ImConfig::loadFile(const char * filename) {
    LoadFile(filename);
}

void ImConfig::saveFile(const char * filename) {
    SaveFile(filename);
}

XMLNode * ImConfig::rootNode() {
    XMLNode * n = FirstChildElement("config");
    if (!n) {
        n = NewElement("config");
        InsertEndChild(n);
    }
    return n;
}

XMLText * ImConfig::getTextNode(const char * name) {
    XMLHandle h = rootNode();
    XMLElement * e = h.FirstChildElement(name).ToElement();
    if (!e) return nullptr; // not exists
    for (XMLNode * it = e->FirstChild(); it; it = it->NextSibling()) {
        XMLText * t = it->ToText();
        if (t) return t;
    }
    XMLText * t = NewText("");
    e->InsertEndChild(t);
    return t;
}

const char * ImConfig::getText(const char * name) {
    XMLText* t = getTextNode(name);
    if (!t) return nullptr;
    return t->Value();
}

void ImConfig::setText(const char * name, const char * text) {
    XMLText* t = getTextNode(name);
    if (t) {
        t->SetValue(text);
    } else {
        XMLElement * node = NewElement(name);
        XMLNode * config = rootNode();
        config->InsertEndChild(node);
        node->InsertEndChild(NewText(text));
    }
}

std::string ImConfig::eval(std::string str) {
    // substitute {} quotation
    for (;;) {
        size_t start = str.find('{');
        size_t end = str.find('}');
        if (start == std::string::npos) break;
        if (end == std::string::npos) break;

        std::string tag = str.substr(start + 1, end - start - 1);
        str.replace(start, end + 1 - start, getText(tag.c_str()));
    }
    return str;
}

}
