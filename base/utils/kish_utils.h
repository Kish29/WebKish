//
// Created by 蒋澳然 on 2021/8/3.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_KISH_UTILS_H
#define WEBKISH_KISH_UTILS_H

#include "string"
#include "map"
#include "vector"

template<class T>
inline T coerce_in(const T &a, const T &b, const T &c) {
    if (a < b) {
        return b;
    }
    if (a > c) {
        return c;
    }
    return a;
}

void split_str1_slow(const std::string &src, const std::string &join_1, std::vector<std::string> &container);

void split_str1(const std::string &src, const std::string &join_1, std::vector<std::string> &container);

void split_str1_in_map(const std::string &src, const std::string &join_1, std::map<std::string, std::string> &container);

void split_str2(const std::string &src, const std::string &join_1, const std::string &join_2, std::map<std::string, std::string> &container);

#endif
