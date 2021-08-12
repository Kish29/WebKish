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
#include "cstring"

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

using std::string;

template<class T>
// 用c++标准库实现的版本，不过略微耗时，慢25%左右
void split_str1_slow(const std::string &src, const std::string &join_1, std::vector<T> &container) {
    string::size_type last_pos = src.find_first_not_of(join_1, 0);
    string::size_type pos = src.find_first_of(join_1, last_pos);

    while (pos != string::npos || last_pos != string::npos) {
        container.emplace_back(src.substr(last_pos, T(pos)));
        last_pos = src.find_first_not_of(join_1, pos);
        pos = src.find_first_of(join_1, last_pos);
    }
}

template<class T>
void split_str1_in_array(const std::string &src, const std::string &join_1, std::vector<T> &container) {
    if (src.empty()) return;
    size_t len = src.length();
    char *ss = new char[len + 1];
    memcpy(ss, src.c_str(), len);
    *(ss + len) = '\0';
    const char *delim = join_1.c_str();
    char *token = strtok(ss, delim);
    while (token) {
        container.emplace_back(T(token));
        token = strtok(nullptr, delim);
    }
    delete[] ss;
}

template<class T, class Container = std::map<string, T>>
void split_str1_in_map(const std::string &src, const std::string &join_1, Container &container) {
    if (src.empty()) return;
    size_t len = src.length();
    char *ss = new char[len + 1];
    memcpy(ss, src.c_str(), len);
    *(ss + len) = '\0';
    const char *delim = join_1.c_str();
    char *token1 = strtok(ss, delim);
    if (token1) {
        char *token2 = strtok(nullptr, delim);
        container.insert(std::make_pair(token1, token2 ? T(string(token2).c_str()) : T()));
    }
    delete[] ss;
}

template<class T, class Container = std::map<string, T>>
void split_str2_in_map(const std::string &src, const std::string &join_1, const std::string &join_2, Container &container) {
    std::vector<string> temp;
    temp.reserve(100);  // assume number

    split_str1_in_array(src, join_1, temp);
    for (const string &s: temp) {
        split_str1_in_map<T, Container>(s, join_2, container);
    }
}

bool kish_atoi(const char *, int *);

bool kish_atof(const char *, double *);

bool kish_atoll(const char *, int64_t *);


char *trim_quote(char *str);

#endif
