#pragma once

#include <cmath>
#include <string>

namespace bc {
const char * number(const char * str, double & result) {
    const char * number_start = str;
    bool negative = false;
    if (*str == '+' | *str == '-') str++;

    while ('0' <= *str && *str <= '9') str++;
    if (*str == '.') {
        str++;
        while ('0' <= *str && *str <= '9') str++;
    }
    if (*str == 'e' || *str == 'E') {
        str++;
        if (*str == '+' | *str == '-') str++;
        if (!('0' <= *str && *str <= '9')) return nullptr;
        while ('0' <= *str && *str <= '9') str++;
    }
    if (1 == sscanf(std::string(number_start, str).c_str(),
            "%lf", &result))
        return str;
    else
        return nullptr;
}
const char * expr(const char * str, double & result);
const char * element(const char * str, double & result) {
    if (*str == '(') {
        str++;
        while (*str == ' ' || *str == '\t') str++;
        str = expr(str, result);
        if (!str) return nullptr;
        while (*str == ' ' || *str == '\t') str++;
        if (*str == ')') str++;
        else return nullptr;
        return str;
    }
    str = number(str, result);
    if (!str) return nullptr;
    return str;
}
const char * factor(const char * str, double & result) {
    str = element(str, result);
    if (!str) return nullptr;
    const char * hend;
hend:
    hend = str;
    while (*str == ' ' || *str == '\t') str++;
    if (*str == '^') {
        str++;
        while (*str == ' ' || *str == '\t') str++;
        double power;
        str = element(str, power);
        if (!str) return nullptr;
        result = pow(result, power);
        goto hend;
    } else str = hend;
    return str;
}
const char * term(const char * str, double & result) {
    str = factor(str, result);
    if (!str) return nullptr;
    const char * hend;
hend:
    hend = str;
    while (*str == ' ' || *str == '\t') str++;
    if (*str == '*' || *str == '/') {
        char op = *str;
        str++;
        while (*str == ' ' || *str == '\t') str++;
        double t;
        str = factor(str, t);
        if (!str) return nullptr;
        if (op == '*') result = result * t;
        if (op == '/') result = result / t;
        goto hend;
    } else str = hend;
    return str;
}
const char * expr(const char * str, double & result) {
    str = term(str, result);
    if (!str) return nullptr;
    const char * hend;
hend:
    hend = str;
    while (*str == ' ' || *str == '\t') str++;
    if (*str == '+' || *str == '-') {
        char op = *str;
        str++;
        while (*str == ' ' || *str == '\t') str++;
        double e;
        str = term(str, e);
        if (!str) return nullptr;
        if (op == '+') result = result + e;
        if (op == '-') result = result - e;
        goto hend;
    } else str = hend;
    return str;
}
bool eval(const char * str, double & result) {
    while (*str == ' ' || *str == '\t') str++;
    str = expr(str, result);
    if (!str) return false;
    while (*str == ' ' || *str == '\t') str++;
    if (*str == '\0') return true;
    else return false;
}
}
