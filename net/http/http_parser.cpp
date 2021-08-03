//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#include "http_parser.h"

#define SPACE   ' '
#define COLON   ':'
#define CR      '\r'
#define LF      '\n'
#define CRLF    '\r\n'

std::string kish::http_request::tostring() const {
    return std::string();
}

std::string kish::http_request::tojson() const {
    return std::string();
}

std::string kish::http_request::tomessage() const {
    return std::string();
}

std::string kish::http_response::tostring() const {
    return std::string();
}

std::string kish::http_response::tojson() const {
    return std::string();
}

std::string kish::http_response::tomessage() const {
    return std::string();
}

bool kish::http_parser::parse_req(kish::http_request &req, const char *begin, const char *end) {
    return false;
}

bool kish::http_parser::parse_resp(kish::http_response &resp, const char *begin, const char *end) {
    return false;
}

