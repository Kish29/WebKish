//
// Created by 蒋澳然 on 2021/8/11.
// email: 875691208@qq.com
// $desc
//

#include "http_transform.h"
#include "kish_utils.h"

kish::http_transform::operator int() const {
    if (number_cast)
        return static_cast<int >(doubleval);
    else throw std::bad_cast();
}

kish::http_transform::operator double() const {
    if (number_cast) return doubleval;
    else throw std::bad_cast();
}

kish::http_transform::operator bool() const {
    if (bool_cast) return boolval;
    else throw std::bad_cast();
}

kish::http_transform::operator const std::string &() const {
    return stringval;
}

kish::http_transform::http_transform(const char *o) {
    if (o) {
        stringval = o;
        if (kish_atof(stringval.c_str(), &doubleval)) {
            number_cast = true;
        }
        if (strcmp(stringval.c_str(), "true") == 0) {
            bool_cast = true;
            boolval = true;
        } else if (strcmp(stringval.c_str(), "false") == 0) {
            bool_cast = true;
            boolval = false;
        }
        if (jsonval.Parse(stringval)) {
            jsonval_cast = true;
        }
    } else throw null_pointer_exception("o");
}

kish::http_transform::operator std::string() {
    return stringval;
}

kish::http_transform::operator const CJsonObject &() const {
    if (jsonval_cast) return jsonval;
    else throw std::bad_cast();
}

kish::http_transform::operator CJsonObject() {
    if (jsonval_cast) return jsonval;
    else throw std::bad_cast();
}

kish::http_transform::operator uint32_t() const {
    if (number_cast)
        return static_cast<uint32_t>(doubleval);
    else throw std::bad_cast();
}

kish::http_transform::operator int64_t() const {
    if (number_cast) return static_cast<int64_t>(doubleval);
    else throw std::bad_cast();
}

kish::http_transform::operator uint64_t() const {
    if (number_cast) return static_cast<uint64_t>(doubleval);
    else throw std::bad_cast();
}
