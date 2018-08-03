#pragma once

#include <tinyxml2.h>
#include <string>

namespace ImConfig {

/* ========== type trait ========== */

template <typename T>
struct type {
    static_assert(sizeof(T) == 0, "unknown type for ImConfig");
    static T default_value();
    static std::string toString(T value);
    static T fromString(const char * str);
};

using namespace tinyxml2;

/* ========== ImConfig ========== */
class ImConfig : XMLDocument {
public:
    template <typename T>
    T get(const char * name);
    template <typename T>
    T get(const char * name, T default_);
    template <typename T>
    void set(const char * name, T value);
    void loadFile(const char * filename);
    void saveFile(const char * filename);
private:
    XMLNode * rootNode(); // get config node, create if not exists
    XMLText * getTextNode(const char * name); // get TextNode for name, null if not exists
    const char * getText(const char * name); // get text of name, null if not exists
    void setText(const char * name, const char * text); // set the value of name, create if not exists
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
        char buf[32];
        sprintf(buf, "%g", value);
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
        char buf[32];
        sprintf(buf, "%lg", value);
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

/* ========== class template implementation ========== */

template <typename T>
T ImConfig::get(const char * name) {
    return get(name, type<T>::default_value());
}

template <typename T>
T ImConfig::get(const char * name, T default_) {
    const char * str = getText(name);
    T v;
    if (str)
        v = type<T>::fromString(str);
    else
        v = default_;
    set(name, v);
    return v;
}

template <typename T>
void ImConfig::set(const char * name, T value) {
    setText(name, type<T>::toString(value).c_str());
}

}
