#pragma once

#include <stdarg.h>
#include <string>
#include <vector>
#include <algorithm>

namespace string_helper {

inline std::vector<std::string> split(const std::string &s, const std::string& delim, const bool keep_empty = true) {
    std::vector<std::string> result;
    if (delim.empty()) {
        result.push_back(s);
        return result;
    }
    std::string::const_iterator substart = s.begin(), subend;
    for (;;) {
        subend = search(substart, s.end(), delim.begin(), delim.end());
        std::string temp(substart, subend);
        if (keep_empty || !temp.empty()) {
            result.push_back(temp);
        }
        if (subend == s.end()) {
            break;
        }
        substart = subend + delim.size();
    }
    return result;
}

inline std::string join(const std::vector<std::string> & ss, const std::string & delim) {
    if (ss.empty()) return "";
    std::string r = ss[0];
    for (size_t i = 1; i < ss.size(); i++) {
        r += delim;
        r += ss[i];
    }
    return r;
}

inline int sprintf(std::string & str, const char * fmt, ...) {
    for (;;) {
        va_list va;
        va_start(va, fmt);
        int l = str.size();
        int n = vsnprintf(&str[0], l, fmt, va);
        va_end(va);
        if (n < l) {
            str.resize(n);
            return n;
        }
        str.resize(n + 1);
    }
}

}
