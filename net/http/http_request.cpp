//
// Created by 蒋澳然 on 2021/8/13.
// email: 875691208@qq.com
// $desc
//

#include "http_request.h"
#include "sstream"
#include "http_parser.h"

std::string kish::http_request::tomessage() {
    std::ostringstream os;
    // version uint8_t must convert to int
    os << llhttp_method_name(method) << SPACE << uri << SPACE << "HTTP/" << (int) ver_major << '.' << (int) ver_minor << CRLF;
    for (const auto &h: headers) {
        os << h.first << COLON << SPACE << h.second << CRLF;
    }
    os << CRLF;
    for (const auto &c : contents) {
        os << c << CRLF;
    }
    if (!contents.empty()) {
        os << CRLF;
    }
    return os.str();
}

void kish::http_request::parse_params_in_uri() {
    size_t question_mark_pos = uri.find_first_of('?');
    string param_str{};
    if (question_mark_pos != string::npos) {
        param_str = uri.substr(question_mark_pos + 1);
        uri = uri.substr(0, question_mark_pos);
    }
    if (!param_str.empty()) {
        split_str2_in_map<http_transform>(param_str, "&", "=", params);
    }
}

void kish::http_request::parse_params_in_contents() {
    // todo: 检查是否合理？
    if (method == HTTP_POST && headers.find(CONTENT_TYPE_KEY) != headers.end()) {
        // url数据
        if (headers.at(CONTENT_TYPE_KEY) == MIME_A_URL) {
            for (const string &c: contents) {
                split_str2_in_map<http_transform>(c, "&", "=", params);
            }
            return;
        }
        // json数据
        if (headers.at(CONTENT_TYPE_KEY) == MIME_A_JSON) {
            for (const string &c: contents) {
                params.insert(std::make_pair(JSON_PARAM, http_transform(c.c_str())));
            }
            return;
        }
        // 纯文本数据
        if (headers.at(CONTENT_TYPE_KEY) == MIME_T_TXT) {
            // todo: 解析post纯文本数据参数
            return;
        }
        // 表单数据
        if (headers.at(CONTENT_TYPE_KEY).find(MIME_F_FORM) != string::npos && !contents.empty()) {
            int minor_mark_len{0};  // 获取前缀符号'-'的长度
            const char *pstr = contents.at(0).c_str();
            while (*pstr == '-') {
                minor_mark_len++;
                pstr++;
            }
            // 构建boundary
            string boundary(minor_mark_len, '-');
            const string &mime = headers.at(CONTENT_TYPE_KEY);
            string::size_type b_pos = mime.find("boundary");
            if (b_pos != string::npos) {
                boundary.append(mime.substr(b_pos + 9));  /* 9 = strlen("boundary=")(0开头，末尾8) + 1*/
            }
            request_multipart_parser parser(boundary);
            for (const string &c: contents) {
                auto res = parser.parse_multipart(c);
                while (!res.empty()) {
                    params.insert(res.back());
                    res.pop();
                }
            }
            return;
        }
        if (headers.at(CONTENT_TYPE_KEY) == MIME_A_XML || headers.at(CONTENT_TYPE_KEY) == MIME_T_XML) {
            // todo: xml标记语言本文参数
            return;
        }
    }
}
