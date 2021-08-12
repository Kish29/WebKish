//
// Created by 蒋澳然 on 2021/8/7.
// email: 875691208@qq.com
// $desc
//


#include "kish_utils.h"
#include "sys/time.h"

void normal_test1() {
    std::string names = "jar asdf safas sad sa safjisoaf sadfjdiosafj";
    std::vector<std::string> tokens;

    split_str1_in_array(names, " ", tokens);

    for (auto const &s: tokens) {
        printf("%s\n", s.c_str());
    }
}

void normal_test2() {
    std::string names = "jar asdf safas sad sa safjisoaf sadfjdiosafj"
                        " jar asdf safas sad sa safjisoaf sadfjdiosafj"
                        " jar asdf safas sad sa safjisoaf sadfjdiosafj"
                        " jar asdf safas sad sa safjisoaf sadfjdiosafj";
    std::vector<std::string> tokens;

    split_str1_in_array(names, " ", tokens);

    for (auto const &s: tokens) {
        printf("%s\n", s.c_str());
    }
}

void normal_test3() {
    std::string names = "jar=1;asdf=3;safas=4;sad=5;sa=7;safjisoaf=99;sadfjdiosafj";
    std::map<std::string, std::string> tokens;

    split_str2_in_map(names, ";", "=", tokens);

    for (auto const &s: tokens) {
        printf("{%s,%s}\n", s.first.c_str(), s.second.c_str());
    }
}

void stress_test() {
    struct timeval start{}, end{};
    gettimeofday(&start, nullptr);

    std::string names = "jar=1;asdf=3;safas=4;sad=5;sa=7;safjisoaf=99;sadfjdiosafj";
    std::vector<std::string> tokens;
    for (int i = 0; i < 1000000; ++i) {
        tokens.clear();
        split_str1_in_array(names, ";", tokens);
    }
    gettimeofday(&end, nullptr);
    size_t d = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("duration is %lu ms\n", d);
}

void stress_test2() {
    struct timeval start{}, end{};
    gettimeofday(&start, nullptr);

    std::string names = "jar=1;asdf=3;safas=4;sad=5;sa=7;safjisoaf=99;sadfjdiosafj";
    std::vector<std::string> tokens;
    for (int i = 0; i < 1000000; ++i) {
        tokens.clear();
        split_str1_slow(names, ";", tokens);
    }
    gettimeofday(&end, nullptr);
    size_t d = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("duration is %lu ms\n", d);
}

#include "cstring"

int main() {
//    normal_test2();
//    normal_test3();
//    stress_test();
//    stress_test2();
    printf("%lu", strlen("username=jiangaoran;password=123456"));
    return 0;
}
