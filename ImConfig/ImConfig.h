#pragma once

#include <string>
#include <map>
#include <functional>
#include <tinyxml2.h>

#include "../string_helper.h"

#ifndef errorfln
#define errorfln(fmt, ...) fprintf(stderr, fmt "\n", __VA_ARGS__)
#endif

namespace ImConfig {

/* ========== type trait ========== */

template <typename T>
struct type {
    static_assert(sizeof(T) == 0, "unknown type for ImConfig");
    static T default_value();
    static std::string toString(T value);
    static T fromString(const char * str);
};

/* ========== traits of some type ========== */

template <>
struct type<int> {
    static int default_value() {return 0;};
    static std::string toString(int value) {
        return std::to_string(value);
    }
    static int fromString(const char * str) {
        int i;
        sscanf(str, "%d", &i);
        return i;
    }
};

template <>
struct type<float> {
    static float default_value() {return 0;};
    static std::string toString(float value) {
        std::string buf;
        string_helper::sprintf(buf, "%g", value);
        return buf;
    }
    static float fromString(const char * str) {
        float f;
        sscanf(str, "%f", &f);
        return f;
    }
};

template <>
struct type<double> {
    static double default_value() {return 0;};
    static std::string toString(double value) {
        std::string buf;
        string_helper::sprintf(buf, "%lg", value);
        return buf;
    }
    static double fromString(const char * str) {
        double d;
        sscanf(str, "%lf", &d);
        return d;
    }
};

template <>
struct type<std::string> {
    using string = std::string;
    static string default_value() {return string();};
    static std::string toString(string value) {
        return value;
    }
    static string fromString(const char * str) {
        return string(str);
    }
};

/* ========== ImConfig ========== */
class ImConfig {
    std::map<std::string, std::string> map;
    std::function<void(const ImConfig &)> cb;
public:
    ~ImConfig() { if (cb) cb(*this); }
    void setCB(std::function<void(const ImConfig &)>f) { cb = f; }
    template <typename T>
    T get(const char * name);
    template <typename T>
    T get(const char * name, T default_);
    template <typename T>
    void set(const char * name, T value);
    const std::map<std::string, std::string> & getMap() const { return map; }
    void modMap(const std::map<std::string, std::string> & m) { map = m; }
    void clear(const char * name);
private:
    const char * getText(const char * name); // get text of name, nullptr if not exists
    void setText(const char * name, const char * text); // set the value of name, create if not exists
};

/* ========== class template implementation ========== */

template <typename T>
T ImConfig::get(const char * name) {
    return get(name, type<T>::default_value());
}

template <typename T>
T ImConfig::get(const char * name, T default_) {
    T v;
    const char * str = getText(name);
    if (str) {
        v = type<T>::fromString(str);
    } else {
        errorfln("WARNING: imconfig: %s does not exist, using default value.", name);
        v = default_;
        set(name, v);
    }
    return v;
}

template <typename T>
void ImConfig::set(const char * name, T value) {
    setText(name, type<T>::toString(value).c_str());
}

using namespace tinyxml2;
class ImConfigFactory {
public:
    static void loadXML(XMLElement * elem, ImConfig & im); // load all entries from XML
    static void saveXML(XMLElement * elem, const ImConfig & im); // write new entries to XML
    ImConfigFactory(const char * fname) {
        filename = fname;
        doc.LoadFile(filename.c_str());
    }
    ~ImConfigFactory() {
        doc.SaveFile(filename.c_str());
    }
    XMLElement * getElement(const char * tag);
    // only new value will be wrote back
    ImConfig createImConfig(const char * tag);
private:
    std::string filename;
    XMLDocument doc;
};

}
