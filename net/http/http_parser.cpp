//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#include "http_parser.h"
#include "logger.h"
#include "sstream"

size_t kish::http_message::content_length() const {
    size_t length = 0;
    for (auto &c: contents) {
        length += c.size();
    }
    return length;
}

std::string kish::http_request::tostring() const {
    return std::string();
}

std::string kish::http_request::tojson() const {
    return std::string();
}

std::string kish::http_request::tomessage() {
    std::ostringstream os;
    // version uint8_t must convert to int
    os << llhttp_method_name(method) << SPACE << uri << SPACE << "HTTP/" << (int) ver_major << '.' << (int) ver_minor << CRLF;
    for (const auto &h: headers) {
        os << h.first << COLON << SPACE << h.second << CRLF;
    }
    os << CRLF;
    if (method == HTTP_POST) {
        for (const auto &c : contents) {
            os << c.size() << CRLF;
            os << c << CRLF;
        }
    }
    if (!contents.empty()) {
        os << CRLF;
    }
    return os.str();
}

std::string kish::http_response::tostring() const {
    return std::string();
}

std::string kish::http_response::tojson() const {
    return std::string();
}

std::string kish::http_response::tomessage() {
    std::ostringstream os;
    os << "HTTP/" << (int) ver_major << '.' << (int) ver_minor << SPACE << status_code << SPACE << short_msg << CRLF;  // request line
    /*⚠️检查是否缺失必要的字段⚠️*/
    if (headers.find(CONTENT_TYPE_KEY) == headers.end()) {
        headers.insert(std::make_pair(CONTENT_TYPE_KEY, "text/plain"));
    }
    // ⚠️Content-Length是必须的，否则客户端没有收到此字段会认为仍然有数据，然后一直等待
    if (headers.find(CONTENT_LENGTH_KEY) == headers.end()) {
        headers.insert(std::make_pair(CONTENT_LENGTH_KEY, std::to_string(content_length())));
    }
    /*if (headers.find(CONNECTION_KEY) == headers.end()) {
        headers.insert(std::make_pair(CONNECTION_KEY, KEEP_ALIVE_VAL));
    }*/
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

kish::http_parser::http_parser(kish::http_parser::parse_type type) : psr_type(type) {
    llhttp_settings_init(&settings);
    settings.on_message_begin = hp_on_message_begin;
    settings.on_url = hp_on_url;
    settings.on_header_field = hp_on_header_field;
    settings.on_header_value = hp_on_header_value;
    settings.on_headers_complete = hp_on_headers_complete;
    settings.on_status = hp_on_status;
    settings.on_body = hp_on_body;
    settings.on_message_complete = hp_on_message_complete;

    llhttp_init(&psr, psr_type == REQUEST ? HTTP_REQUEST : HTTP_RESPONSE, &settings);
    psr.data = this;
}

void kish::http_parser::reset() {
    method = HTTP_GET;
    ver_major = 1;
    ver_minor = 0;
    stat_code = 400;
    shrt_msg = RESP_STAT_CODE_MAP.at(400);
    uri.clear();
    headers.clear();
    header_field.clear();
    header_value.clear();
    last_on_hd = IDLE;
    kep_alv = KEEP_ALIVE;
    contents.clear();
    message_complete = false;

    // todo: 0.0.1版本问题，设计一个更好的接口，以达到增量parse的效果
    llhttp_init(&psr, psr_type == REQUEST ? HTTP_REQUEST : HTTP_RESPONSE, &settings);
    psr.data = this;
}

kish::http_parser::parse_res kish::http_parser::parse(const char *raw, size_t len) {
    if (!raw || len <= 0) return http_parser::PARSE_FAIL;
    reset();
    // 使用 llhttp_execute 进行解析
    enum llhttp_errno err = llhttp_execute(&psr, raw, len);
    // ⚠️注意，先判断是否出错，然后是否继续输入请求，最后判断是否成功
    if (err != HPE_OK) {
        return http_parser::PARSE_FAIL;
    } else if (!message_complete) {
        return http_parser::PARSE_GO_ON;
    } else if (err == HPE_OK) {
        return http_parser::PARSE_SUCCESS;
    } else {
        return http_parser::PARSE_FAIL;
    }
}

int kish::http_parser::on_message_begin(llhttp_t *parser) {
    parse_time++;
    return 0;
}

int kish::http_parser::on_url(llhttp_t *parser, const char *at, size_t len) {
    // todo: 解析GET请求路径中的参数
    uri = string(at, len);
    return 0;
}

int kish::http_parser::on_status(llhttp_t *parser, const char *at, size_t len) {
    string code(at, len);
    stat_code = atoi(code.c_str());
    return 0;
}

int kish::http_parser::on_header_field(llhttp_t *parser, const char *at, size_t len) {
    switch (last_on_hd) {
        case IDLE:
            header_field = string(at, len);
            break;
        case FIELD:
            header_field.append(string(at, len));
            break;
        case VALUE:
            headers.insert(std::make_pair(header_field, header_value));
            // 仍然需要更新
            header_field = string(at, len);
            break;
    }
    last_on_hd = FIELD;
    return 0;
}

int kish::http_parser::on_header_value(llhttp_t *parser, const char *at, size_t len) {
    switch (last_on_hd) {
        case IDLE:
            LOG_INFO << "header value error: before field value comes in";
            break;
        case FIELD:
            // 注意，只能有一边更新
            header_value = string(at, len);
            break;
        case VALUE:
            header_value.append(string(at, len));
            break;
    }
    last_on_hd = VALUE;
    return 0;
}

int kish::http_parser::on_headers_complete(llhttp_t *parser) {
    // 添加最后一个header
    if (last_on_hd == VALUE) {
        headers.emplace(std::make_pair(header_field, header_value));
        last_on_hd = IDLE;
    }
    return 0;
}

int kish::http_parser::on_body(llhttp_t *parser, const char *at, size_t len) {
    contents.emplace_back(string(at, len));
    return 0;
}

int kish::http_parser::on_message_complete(llhttp_t *parser) {
    message_complete = true;
    method = static_cast<llhttp_method_t>(parser->method);
    ver_major = parser->http_major;
    ver_minor = parser->http_minor;
    if (RESP_STAT_CODE_MAP.find(stat_code) != RESP_STAT_CODE_MAP.end()) {
        shrt_msg = RESP_STAT_CODE_MAP.at(stat_code);
    }
    string cnn;
    if (headers.find(CONNECTION_KEY) != headers.end()) {
        cnn = headers.at(CONNECTION_KEY);
    }

    // 设置连接类型
    size_t timeout_pos = string::npos;
    if (!cnn.empty()) {
        timeout_pos = cnn.find("timeout");
    }

    if (llhttp_should_keep_alive(parser)) {
        kep_alv = KEEP_ALIVE;
    } else if (timeout_pos != string::npos) {
        kep_alv = SET_TIMEOUT;
        string to = cnn.substr(timeout_pos + 8/* strlen(timeout=) + 1 */);
        timeout = atoi(to.c_str());
    } else {
        kep_alv = CLOSE_IMM;
    }

    return 0;
}

kish::http_request_ptr kish::http_parser::get_req() {
    if (psr_type == REQUEST) {
        std::shared_ptr<http_request> request = std::make_shared<http_request>();
        request->method = this->method;
        request->uri = this->uri;
        request->ver_major = this->ver_major;
        request->ver_minor = this->ver_minor;
        request->headers = this->headers;
        request->contents = this->contents;
        request->alive = this->kep_alv;
        request->timeout = this->timeout;
        return request;
    } else return nullptr;
}

kish::http_response_ptr kish::http_parser::get_resp() {
    if (psr_type == RESPONSE) {
        std::shared_ptr<http_response> response = std::make_shared<http_response>();
        response->status_code = this->stat_code;
        response->short_msg = shrt_msg;
        response->ver_major = this->ver_major;
        response->ver_minor = this->ver_minor;
        response->headers = this->headers;
        response->contents = this->contents;
        response->alive = this->kep_alv;
        response->timeout = this->timeout;
        return response;
    } else return nullptr;
}
