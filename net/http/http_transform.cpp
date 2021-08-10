//
// Created by 蒋澳然 on 2021/8/11.
// email: 875691208@qq.com
// $desc
//

#include "http_transform.h"

kish::http_transform::operator const std::vector<http_transform> &() const {
    if (child.empty()) throw std::bad_cast();
    return child;
}

kish::http_transform::operator int() const {
    if (!(type & cJSON_Number)) throw std::bad_cast();
    if (type == cJSON_Number_Integer) {
        return intval;
    } else {
        return static_cast<int>(doubleval);
    }
}

kish::http_transform::operator const std::vector<int> &() const {
    if (type != cJSON_Array) throw std::bad_cast();
    return integers;
}

kish::http_transform::operator char() const {
    return static_cast<char>(intval);
}

kish::http_transform::operator const char *() const {
    return ((const std::string &) *this).c_str();
}

kish::http_transform::operator double() const {
    if (!(type & cJSON_Number)) throw std::bad_cast();
    if (type == cJSON_Number_Integer) {
        return static_cast<double>(intval);
    } else {
        return doubleval;
    }
}

kish::http_transform::operator const std::vector<double> &() const {
    if (type != cJSON_Array) throw std::bad_cast();
    return doubles;
}

kish::http_transform::operator bool() const {
    if (type == cJSON_True) return true;
    else if (type == cJSON_False) return false;
    else throw std::bad_cast();
}

kish::http_transform::operator const std::string &() const {
    if (type != cJSON_String) throw std::bad_cast();
    return stringval;
}

kish::http_transform::operator const std::vector<std::string> &() const {
    if (type != cJSON_Array) throw std::bad_cast();
    return strings;
}

std::string kish::http_transform::tojson() const {
    return std::string();
}

