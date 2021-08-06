//
// Created by 蒋澳然 on 2021/8/7.
// email: 875691208@qq.com
// $desc
//

#include "kish_utils.h"
#include "cstring"

using std::string;

// 用c++标准库实现的版本，不过略微耗时，慢25%左右
void split_str1_slow(const std::string &src, const std::string &join_1, std::vector<std::string> &container) {
    string::size_type last_pos = src.find_first_not_of(join_1, 0);
    string::size_type pos = src.find_first_of(join_1, last_pos);

    while (pos != string::npos || last_pos != string::npos) {
        container.emplace_back(src.substr(last_pos, pos));
        last_pos = src.find_first_not_of(join_1, pos);
        pos = src.find_first_of(join_1, last_pos);
    }
}

void split_str1(const std::string &src, const std::string &join_1, std::vector<std::string> &container) {
    if (src.empty()) return;
    size_t len = src.length();
    char *ss = new char[len + 1];
    memcpy(ss, src.c_str(), len);
    *(ss + len) = '\0';
    const char *delim = join_1.c_str();
    char *token = strtok(ss, delim);
    while (token) {
        container.emplace_back(token);
        token = strtok(nullptr, delim);
    }
    delete[] ss;
}

void split_str1_in_map(const std::string &src, const std::string &join_1, std::map<string, string> &container) {
    if (src.empty()) return;
    size_t len = src.length();
    char *ss = new char[len + 1];
    memcpy(ss, src.c_str(), len);
    *(ss + len) = '\0';
    const char *delim = join_1.c_str();
    char *token1 = strtok(ss, delim);
    if (token1) {
        char *token2 = strtok(nullptr, delim);
        container.insert(std::make_pair(token1, token2 ? token2 : ""));
    }
    delete[] ss;
}

void split_str2(const std::string &src, const std::string &join_1, const std::string &join_2, std::map<std::string, std::string> &container) {
    std::vector<string> temp;
    temp.reserve(100);  // assume number

    split_str1(src, join_1, temp);
    for (const string &s: temp) {
        split_str1_in_map(s, join_2, container);
    }
}
