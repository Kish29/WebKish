//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#include "http_parser.h"
#include "logger.h"

kish::http_parser::http_parser(kish::http_parser::parse_type type) : psr_type(type) {
    llhttp_settings_init(&settings);
    settings.on_message_begin = hp_on_message_begin;
    settings.on_url = hp_on_url;
    settings.on_url_complete = hp_on_url_complete;
    settings.on_header_field = hp_on_header_field;
    settings.on_header_value = hp_on_header_value;
    settings.on_headers_complete = hp_on_headers_complete;
    settings.on_status = hp_on_status;
    settings.on_status_complete = hp_on_status_complete;
    settings.on_body = hp_on_body;
    settings.on_message_complete = hp_on_message_complete;

    llhttp_init(&psr, psr_type == REQUEST ? HTTP_REQUEST : HTTP_RESPONSE, &settings);
    psr.data = this;
}

void kish::http_parser::reset() {
    method = HTTP_GET;
    uri.clear();
    headers.clear();
    header_field.clear();
    header_value.clear();
    last_on_hd = IDLE;
    alive = KEEP_ALIVE;
    contents.clear();

    llhttp_reset(&psr);
}

void kish::http_parser::parse(const char *raw, size_t len) {
    if (!raw || len <= 0) {
        on_error();
        reset();
        return;
    } else {
        // 检查上一次解析的结构是否完成
        if (last_parse_complete) {
            // 如果完成，重置资源
            last_parse_complete = false;
            reset();
        }
        enum llhttp_errno err = llhttp_execute(&psr, raw, len);
        if (err != HPE_OK) {
            reset();
            on_error();
            // todo: 这儿仅仅做了日志的记录
            LOG_TRACE << "llhttp parse error: " << llhttp_errno_name(err) << " reason: " << llhttp_get_error_reason(&psr);
        }
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
            LOG_INFO << "header value_t error: before field value_t comes in";
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
    // headers完成后，设置连接类型
    string cnn;
    if (headers.find(CONNECTION_KEY) != headers.end()) {
        cnn = headers.at(CONNECTION_KEY);
    }
    size_t timeout_pos = string::npos;
    if (!cnn.empty()) {
        timeout_pos = cnn.find("timeout");
    }
    if (llhttp_should_keep_alive(parser) && timeout_pos == string::npos) {
        // 这里光依赖llhttp的判断是不可靠的，因为设置了timeout会被llhttp忽略掉
        alive = KEEP_ALIVE;
    } else if (timeout_pos != string::npos) {
        alive = SET_TIMEOUT;
        string to = cnn.substr(timeout_pos + 8);    /* 8 = strlen(timeout=) + 1 */
        if (!kish_atoi(to.c_str(), &timeout)) {
            LOG_INFO << "timeout atoi convert failed";
        }
    } else {
        alive = CLOSE_IMM;
    }
    return 0;
}

int kish::http_parser::on_body(llhttp_t *parser, const char *at, size_t len) {
    contents.emplace_back(string(at, len));
    return 0;
}

int kish::http_parser::on_message_complete(llhttp_t *parser) {
    return 0;
}

int kish::http_parser::on_url_complete(llhttp_t *parser) {
    return 0;
}

int kish::http_parser::on_status_complete(llhttp_t *parser) {
    return 0;
}

int kish::http_request_parser::on_url_complete(llhttp_t *parser) {
    assert(request);
    request->method = static_cast<llhttp_method_t> (parser->method);
    request->uri.swap(base::uri);
    // 解析uri中可能携带的参数
    request->parse_params_in_uri();
    // 通知调用者 uri 解析成功
    cb_on_uri(request);
    return 0;
}

int kish::http_request_parser::on_headers_complete(llhttp_t *parser) {
    base::on_headers_complete(parser);

    assert(request);
    request->headers.swap(headers);
    request->ver_major = parser->http_major;
    request->ver_minor = parser->http_minor;
    parse_control res = cb_on_headers(request);
    switch (res) {
        case GO_ON:
            return 0;
        case SKIP:
            return 1;
        case ERROR:
            return -1;
    }
    // no reachable here
    return 0;
}

int kish::http_request_parser::on_message_complete(llhttp_t *parser) {
    assert(request);
    // 注意：有可能一个GET请求没有Header部分，在这里再次赋值，确保版本更新
    request->ver_major = parser->http_major;
    request->ver_minor = parser->http_minor;
    request->contents.swap(contents);
    // 解析contents总可能携带的参数
    request->parse_params_in_contents();
    request->alive = alive;
    request->timeout = timeout;
    cb_on_complete(request);

    // 一次报文解析成功，重置状态
    last_parse_complete = true;
    return 0;
}

int kish::http_response_parser::on_status_complete(llhttp_t *parser) {
    assert(response);
    response->update_stat(parser->status_code);
    cb_on_resp_line(response);
    return base::on_status_complete(parser);
}

int kish::http_response_parser::on_headers_complete(llhttp_t *parser) {
    base::on_headers_complete(parser);

    assert(response);
    response->ver_major = parser->http_major;
    response->ver_minor = parser->http_minor;
    response->headers.swap(headers);
    response->parse_params_in_contents();
    parse_control res = cb_on_headers(response);
    switch (res) {
        case GO_ON:
            return 0;
        case SKIP:
            return 1;
        case ERROR:
            return -1;
    }
    // no reachable here
    return 0;
}

int kish::http_response_parser::on_message_complete(llhttp_t *parser) {
    assert(response);

    response->ver_major = parser->http_major;
    response->ver_minor = parser->http_minor;
    response->contents.swap(contents);
    response->alive = alive;
    response->timeout = timeout;
    cb_on_complete(response);

    last_parse_complete = true;
    return 0;
}


void kish::http_request_parser::reset() {
    base::reset();
    request = std::make_shared<http_request>();
}

void kish::http_response_parser::reset() {
    base::reset();
    response = std::make_shared<http_response>();
}

kish::param_container &kish::request_multipart_parser::parse_multipart(const char *body, size_t bodylen) {
    params.clear();
    multipart_parser_execute(m_parser, body, bodylen);
    return params;
}

int kish::request_multipart_parser::on_read_header_value(multipart_parser *parser, const char *at, size_t len) {
    char *buf = new char[len + 1];
    memcpy(buf, at, len);
    *(buf + len) = '\0';
    char *fld = strrchr(buf, '=');
    switch (last_s) {
        case LAST_VALUE:
        case IDLE: {
            if (fld) {
                fld++;
                fld = trim_quote(fld);
                field = fld;
            }
            break;
        }
        case LAST_FIELD:
            /*if (fld) {
                fld++;
                fld = trim_quote(fld);
                field.append(fld);
            }*/
            // fixme: 这儿只取第一个key
            break;
    }
    delete[] buf;
    last_s = LAST_FIELD;
    return 0;
}

int kish::request_multipart_parser::on_part_data(multipart_parser *parser, const char *at, size_t len) {
    char *buf = new char[len + 1];
    memcpy(buf, at, len);
    *(buf + len) = '\0';
    switch (last_s) {
        case IDLE:
            LOG_INFO << "something must be wrong with this request message\n";
            break;
        case LAST_FIELD: {
            value = buf;
            params.insert(std::make_pair(field, http_transform(value.c_str())));
            break;
        }
        case LAST_VALUE:
            value.append(buf);
            break;
    }
    delete[] buf;
    last_s = LAST_VALUE;
    return 0;
}

std::unordered_map<string, string> mime_types = {
        {".html",      MIME_T_HTML},
        {".htm",       MIME_T_HTM},
        {".txt",       MIME_T_TXT},
        {".c",         MIME_T_C},
        {".bmp",       MIME_M_BMP},
        {".ico",       MIME_M_ICO},
        {".jpg",       MIME_M_JPG},
        {".png",       MIME_M_PNG},
        {".gif",       MIME_M_GIF},
        {".gz",        MIME_A_GZ},
        {".xml",       MIME_T_XML},
        {".avi",       MIME_M_AVI},
        {".mp3",       MIME_M_MP3},
        {".doc",       MIME_A_DOC},
        {MIME_DEFAULT, MIME_T_TXT}
};

const char *get_file_mime_type(const char *filename) {
    if (!filename) return mime_types.at(MIME_DEFAULT).c_str();
    const char *sfx = strrchr(filename, '.');
    if (sfx && mime_types.find(sfx) != mime_types.end()) {
        return mime_types.at(sfx).c_str();
    }
    return mime_types.at(MIME_DEFAULT).c_str();
}
