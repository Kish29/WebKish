//
// Created by 蒋澳然 on 2021/8/13.
// email: 875691208@qq.com
// $desc
//

#include "http_response.h"
#include "sstream"

std::string kish::http_response::tomessage() {
    std::ostringstream os;
    os << "HTTP/" << (int) ver_major << '.' << (int) ver_minor << SPACE << status_code << SPACE << short_msg << CRLF;  // request line
    /*⚠️检查是否缺失必要的字段⚠️*/
    if (headers.find(CONTENT_TYPE_KEY) == headers.end()) {
        headers.insert(std::make_pair(CONTENT_TYPE_KEY, MIME_T_TXT"; charset=UTF-8"));
    }
    // ⚠️Content-Length是必须的，否则客户端没有收到此字段会认为仍然有数据，然后一直等待
    if (headers.find(CONTENT_LENGTH_KEY) == headers.end()) {
        headers.insert(std::make_pair(CONTENT_LENGTH_KEY, std::to_string(content_length())));
    }
    for (const auto &h: headers) {
        os << h.first << COLON << SPACE << h.second << CRLF;
    }
    os << CRLF;
    for (const auto &c : contents) {
        os << c;
    }
    if (!contents.empty()) {
        os << CRLF;
    }
    return os.str();
}
