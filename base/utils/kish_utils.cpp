#include "kish_utils.h"

#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define IS_CTRL(c)  ((c) > 0 && (c) <= 32)

bool kish_atoi(const char *source, int *integer) {
    if (!source || !integer || *source == 0) return false;
    int64_t temp{};
    int64_t *p = &temp;
    if (kish_atoll(source, p)) {
        *integer = static_cast<int>(temp);
        return true;
    } else return false;
}

bool kish_atof(const char *source, double *double_num) {
    if (!source || !double_num || *source == 0) return false;
    const char *pstr = source;
    while (IS_CTRL(*pstr)) {   // 移除控制字符，'\0'除外
        pstr++;
    }
    bool signed_flag = true;
    if (*pstr == '-') {
        signed_flag = false;
        pstr++;
    }
    // 整数部分
    double num{0};
    bool converted = false;
    while (IS_DIGIT(*pstr)) {
        num = *pstr - '0' + num * 10.0;
        pstr++;
        converted = true;
    }
    // 小数部分
    if (converted && *pstr == '.') {
        pstr++;
        // 次方
        int n{10};
        while (IS_DIGIT(*pstr)) {
            num = (*pstr - '0') * (1.0 / n) + num;
            n *= 10;
            pstr++;
        }
    }
    if (converted) {
        *double_num = signed_flag ? num : -num;
        return true;
    }
    return false;
}

bool kish_atoll(const char *source, int64_t *i64_t) {
    if (!source || !i64_t || *source == 0) return false;
    const char *pstr = source;
    while (IS_CTRL(*pstr)) {   // 移除控制字符
        pstr++;
    }
    bool signed_flag = true;     // true -> + / false -> -
    if (*pstr == '-') {
        signed_flag = false;
        pstr++;
    }
    int64_t num{0};
    bool converted = false;
    while (IS_DIGIT(*pstr)) {
        num = *pstr - '0' + num * 10;
        pstr++;
        converted = true;
    }
    if (converted) {
        *i64_t = signed_flag ? num : -num;
        return true;
    }
    return false;
}

char *trim_quote(char *str) {
    while (*str == '"' || *str == '\\') str++;
    size_t len = strlen(str);
    while (*(str + len - 1) == '"' || *str == '\\') len--;
    *(str + len) = '\0';
    return str;
}

#define IS_ALPHA(c)     (((c) >= 65 && (c) <= 90) || ((c) >=97 && (c) <= 122))

std::string kish_toupper(const string &str) {
    if (!str.empty()) {
        char *res = new char[str.size() + 1];
        for (int i = 0; i < str.size(); i++) {
            if (IS_ALPHA(str.at(i))) {
                res[i] = (str.at(i) & 0xdf);
            } else res[i] = str.at(i);
        }
        *(res + str.size()) = '\0';
        std::string r(res);
        delete[] res;
        return r;
    }
    return str;
}

std::string kish_tolower(const string &str) {
    if (!str.empty()) {
        char *res = new char[str.size() + 1];
        for (int i = 0; i < str.size(); i++) {
            if (IS_ALPHA(str.at(i))) {
                res[i] = (str.at(i) | 0x20);
            } else res[i] = str.at(i);
        }
        *(res + str.size()) = '\0';
        std::string r(res);
        delete[] res;
        return r;
    }
    return str;
}
