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
    const char * str = t->Value();
    return str;
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

}
