//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_PARSER_H
#define WEBKISH_HTTP_PARSER_H

#include "base.h"
#include "map"
#include "llhttp.h"
#include "kish_utils.h"
#include "vector"
#include "http_transform.h"
#include "list_map.h"

const static std::map<int, std::string> RESP_STAT_CODE_MAP = {
// see https://developer.mozilla.org/en-US/docs/Web/HTTP/Status#client_error_responses
        {100, "Continue"},
        {101, "Switching Protocols"},
        {102, "Processing"},
        {103, "Early Hints"},
        {200, "OK"},
        {201, "Created"},
        {202, "Accepted"},
        {203, "Non-Authoritative Information"},
        {204, "No Content"},
        {205, "Reset Content"},
        {206, "Partial Content"},
        {300, "Multiple Choices"},
        {301, "Moved Permanently"},
        {302, "Found"},
        {303, "See Other"},
        {304, "Not Modified"},
        {305, "Use Proxy"},
        {307, "Temporary Redirect"},
        {308, "Permanent Redirect"},
        {400, "Bad Request"},
        {401, "Unauthorized"},
        {402, "Payment Required"},
        {403, "Forbidden"},
        {404, "Not Found"},
        {405, "Method Not Allowed"},
        {406, "Not Acceptable"},
        {407, "Proxy Authentication Required"},
        {408, "Request Timeout"},
        {409, "Conflict"},
        {410, "Gone"},
        {411, "Length Required"},
// ...
        {500, "Internal Server Error"},
        {501, "Not Implemented"},
        {502, "Bad Gateway"},
        {503, "Service Unavailable"},
        {504, "Gateway Timeout"},
        {505, "HTTP Version Not Supported"},
        {506, "Variant Also Negotiates"},
        {507, "Insufficient Storage"},
        {508, "Loop Detected"},
        {510, "Not Extended"},
        {511, "Network Authentication Required"}
};

// T[text] text type
#define MIME_T_HTML     "text/html"
#define MIME_T_TXT      "text/plain"
#define MIME_T_XML      "text/xml"
#define MIME_T_C        "text/plain"
#define MIME_T_HTM      "text/html"
#define MIME_T_DFT      "text/plain"

// M[media] media type
#define MIME_M_AVI      "video/x-msvideo"
#define MIME_M_MP3      "audio/mp3"

#define MIME_M_BMP      "image/bmp"
#define MIME_M_GIF      "image/gif"
#define MIME_M_ICO      "image/x-icon"
#define MIME_M_JPG      "image/jpeg"
#define MIME_M_PNG      "image/png"

// A[application] application type
#define MIME_A_GZ       "application/x-gzip"
#define MIME_A_DOC      "application/msword"
#define MIME_A_URL      "application/x-www-form-urlencoded"
#define MIME_A_PDF      "application/pdf"
#define MIME_A_XML      "application/xml"
#define MIME_A_JSON     "application/json"
#define MIME_A_XHTML    "application/xhtml+xml"
#define MIME_A_BSTRM    "application/octet-stream"

// F[form]  form type
#define MIME_F_FORM     "multipart/form-data"

#define SPACE   ' '
#define COLON   ':'
#define CRLF    "\r\n"

#define CONNECTION_KEY      "Connection"
#define KEEP_ALIVE_VAL      "Keep-Alive"
#define CLOSE_VAL           "close"
#define CONTENT_TYPE_KEY    "Content-Type"
#define CONTENT_LENGTH_KEY  "Content-Length"
#define TRANSFER_TYPE_KEY   "Transfer-Encoding"

namespace kish {
    using std::string;

    enum keep_alive_t {
        CLOSE_IMM,      // close immediate
        KEEP_ALIVE, // keep alive forever
        SET_TIMEOUT // set timeout
    };

    typedef list_map<string, http_transform> param_container;

    struct http_message : copyable, public message_type {
        typedef std::map<std::string, std::string> header_item;

        uint8_t ver_major;
        uint8_t ver_minor;
        header_item headers{};
        std::vector<string> contents{};

        keep_alive_t alive;
        // 如果 alive 类型为 has_set_timeout，该字段会被设置
        uint32_t timeout{60};

        param_container params{};

        const http_transform &get_param(const string &key);

        virtual void parse_params_in_contents();

        void set_content_length(size_t len);

        size_t content_length() const;

        void set_content_type(const string &type);

        const string &get_content_type() const;

        void set_transfer_encoding(const string &enc);

        const string &get_transfer_encoding() const;

        http_message() : ver_major(1), ver_minor(0), alive(KEEP_ALIVE) {};

        static const std::string NO_CONTENT_TYPE;
        static const std::string NO_TRANSFER_ENCODE;
        static const std::string JSON_PARAM;
        static const std::string EMPTY_PARAM;
    };

    struct http_request : public http_message {
        llhttp_method_t method;
        // uri中有可能携带参数
        std::string uri;

        void parse_params_in_uri();

        http_request() : http_message() {
            method = HTTP_GET;
            uri = "/";
        }

        const string &get_host() const;

        const string &set_host() const;

        void parse_params_in_contents() override;

        http_request(llhttp_method_t meth, string u) : method(meth), uri(std::move(u)) {}

        std::string tomessage() override;
    };

    typedef std::shared_ptr<http_request> http_request_ptr;

    struct http_response : public http_message {
        int status_code;
        std::string short_msg;

        http_response() : http_message() {
            status_code = 404;
            // todo: response code map
            short_msg = RESP_STAT_CODE_MAP.at(404);
        }

        http_response(int code, string short_msg) : status_code(code), short_msg(std::move(short_msg)) {}

        void update_stat(int stat_code) {
            if (RESP_STAT_CODE_MAP.find(stat_code) != RESP_STAT_CODE_MAP.end()) {
                short_msg = RESP_STAT_CODE_MAP.at(stat_code);
            }
            status_code = stat_code;
        }

        std::string tomessage() override;
    };

    typedef std::shared_ptr<http_response> http_response_ptr;

    enum parse_control {
        GO_ON,      // 表示调用者检测通过，继续解析
        SKIP,       // 表示调用者跳过此报文的解析    --->  调用message_complete
        ERROR       // 表示调用者检测失败，parser内部会重置所有的状态和资源
    };

    class http_parser : noncopyable {
        typedef enum { IDLE, FIELD, VALUE } last_on_header;
    public:
        // 解析次数
        int parse_time{0};

    public:
        void set_on_parse_error(const callable &cb) {
            on_error = cb;
        }

        virtual void parse(const char *raw, size_t len);

    protected:
        enum parse_type {
            REQUEST,
            RESPONSE
        };

        explicit http_parser(parse_type type);

    protected:
        parse_type psr_type;
        llhttp_t psr{};
        llhttp_settings_t settings{};

        // 这个是request和response的公共部分
        callable on_error{};
        llhttp_method_t method{HTTP_GET};
        string uri{};
        http_message::header_item headers{};
        // 用enum和两个string记录一对键值对
        string header_field{}, header_value{};
        last_on_header last_on_hd{IDLE};
        keep_alive_t alive{KEEP_ALIVE};
        int timeout{0};
        std::vector<string> contents{};

        // ⚠️重要，指示上一次报文是否解析完成，以在报文解析开始前选择是否重置资源
        bool last_parse_complete{false};


#define HP reinterpret_cast<http_parser*>(parser->data)
    protected:
        static int hp_on_message_begin(llhttp_t *parser) { return HP->on_message_begin(parser); }

        static int hp_on_url(llhttp_t *parser, const char *at, size_t len) { return HP->on_url(parser, at, len); }

        static int hp_on_url_complete(llhttp_t *parser) { return HP->on_url_complete(parser); }

        static int hp_on_status(llhttp_t *parser, const char *at, size_t len) { return HP->on_status(parser, at, len); }

        static int hp_on_status_complete(llhttp_t *parser) { return HP->on_status_complete(parser); }

        static int hp_on_header_field(llhttp_t *parser, const char *at, size_t len) { return HP->on_header_field(parser, at, len); }

        static int hp_on_header_value(llhttp_t *parser, const char *at, size_t len) { return HP->on_header_value(parser, at, len); }

        static int hp_on_headers_complete(llhttp_t *parser) { return HP->on_headers_complete(parser); }

        static int hp_on_body(llhttp_t *parser, const char *at, size_t len) { return HP->on_body(parser, at, len); }

        static int hp_on_message_complete(llhttp_t *parser) { return HP->on_message_complete(parser); }

#undef HP

    protected:
        // reset可以让parse被多次调用
        virtual void reset();

        virtual int on_message_begin(llhttp_t *parser);

        virtual int on_url(llhttp_t *parser, const char *at, size_t len);

        virtual int on_url_complete(llhttp_t *parser);

        virtual int on_status(llhttp_t *parser, const char *at, size_t len);

        virtual int on_status_complete(llhttp_t *parser);

        virtual int on_header_field(llhttp_t *parser, const char *at, size_t len);

        virtual int on_header_value(llhttp_t *parser, const char *at, size_t len);

        virtual int on_headers_complete(llhttp_t *parser);

        virtual int on_body(llhttp_t *parser, const char *at, size_t len);

        virtual int on_message_complete(llhttp_t *parser);

    };

    typedef std::function<void(const http_request_ptr &request)> request_parse_callback;
    typedef std::function<parse_control(const http_request_ptr &request)> request_header_callback;

    class http_request_parser : public http_parser {
        typedef http_parser base;
    public:
        http_request_parser() : http_parser(http_parser::REQUEST) {}

        void set_on_request_uri_parse_complete(const request_parse_callback &cb) {
            cb_on_uri = cb;
        }

        void set_on_headers_parse_complete(const request_header_callback &cb) {
            cb_on_headers = cb;
        }

        void set_on_message_parser_complete(const request_parse_callback &cb) {
            cb_on_complete = cb;
        }

    private:
        http_request_ptr request{new http_request};
        request_parse_callback cb_on_uri{};
        request_header_callback cb_on_headers{};
        request_parse_callback cb_on_complete{};

    private:

        void reset() override;

        // request 重写这四个
        int on_url_complete(llhttp_t *parser) override;

        int on_headers_complete(llhttp_t *parser) override;

        int on_message_complete(llhttp_t *parser) override;

    };

    typedef std::function<parse_control(const http_response_ptr &response)> response_parse_callback;
    typedef std::function<parse_control(const http_response_ptr &request)> response_header_callback;

    class http_response_parser : public http_parser {
        typedef http_parser base;
    public:
        http_response_parser() : http_parser(http_parser::RESPONSE) {}

        void set_on_response_line_parse_complete(const response_parse_callback &cb) {
            cb_on_resp_line = cb;
        }

        void set_on_headers_parse_complete(const response_header_callback &cb) {
            cb_on_headers = cb;
        }

        void set_on_message_parser_complete(const response_parse_callback &cb) {
            cb_on_complete = cb;
        }

    private:
        http_response_ptr response{new http_response};
        response_parse_callback cb_on_resp_line;
        response_header_callback cb_on_headers;
        response_parse_callback cb_on_complete;

    private:

        void reset() override;

        // response重写这三个
        int on_status_complete(llhttp_t *parser) override;

        int on_headers_complete(llhttp_t *parser) override;

        int on_message_complete(llhttp_t *parser) override;

    };

    class http_request_builder : noncopyable {
    public:
        // 重载类型转换
        explicit operator http_request() {
            return request;
        }

        http_request &build() {
            return request;
        }

        http_request_builder &method(const llhttp_method_t &mtd) {
            request.method = mtd;
            return *this;
        }

        http_request_builder &uri(const string &uri) {
            request.uri = uri;
            return *this;
        }

        http_request_builder &uri(string &&uri) {
            request.uri = std::move(uri);
            return *this;
        }

        http_request_builder &ver_major(uint8_t major) {
            request.ver_major = major;
            return *this;
        }

        http_request_builder &ver_minor(uint8_t minor) {
            request.ver_minor = minor;
            return *this;
        }

        http_request_builder &keep_alive(bool keep) {
            request.alive = keep ? KEEP_ALIVE : CLOSE_IMM;
            return *this;
        }

        http_request_builder &timeout(uint32_t time) {
            request.alive = SET_TIMEOUT;
            request.timeout = time;
            return *this;
        }

        http_request_builder &header(const string &key, const string &val) {
            request.headers.insert(std::make_pair(key, val));
            return *this;
        }

        http_request_builder &header(string &&key, string &&val) {
            request.headers.insert(std::make_pair(std::move(key), std::move(val)));
            return *this;
        }

        http_request_builder &content(const string &data) {
            request.contents.emplace_back(data);
            return *this;
        }

        http_request_builder &content(string &&data) {
            request.contents.emplace_back(std::move(data));
            return *this;
        }

    private:
        http_request request;
    };

    class http_response_builder : noncopyable {
    public:

        // 重载类型转换
        explicit operator http_response() {
            return response;
        }

        http_response &build() {
            return response;
        }

        // 设置status_code 会自动生成short_msg
        http_response_builder &status_code(int code) {
            response.status_code = code;
            if (RESP_STAT_CODE_MAP.find(code) != RESP_STAT_CODE_MAP.end()) {
                response.short_msg = RESP_STAT_CODE_MAP.at(code);
            }
            return *this;
        }

        http_response_builder &short_msg(const string &msg) {
            response.short_msg = msg;
            return *this;
        }

        http_response_builder &uri(string &&msg) {
            response.short_msg = std::move(msg);
            return *this;
        }

        http_response_builder &ver_major(uint8_t major) {
            response.ver_major = major;
            return *this;
        }

        http_response_builder &ver_minor(uint8_t minor) {
            response.ver_minor = minor;
            return *this;
        }

        http_response_builder &keep_alive(bool keep) {
            response.alive = keep ? KEEP_ALIVE : CLOSE_IMM;
            return *this;
        }

        http_response_builder &timeout(uint32_t time) {
            response.alive = SET_TIMEOUT;
            response.timeout = time;
            return *this;
        }

        http_response_builder &header(const string &key, const string &val) {
            response.headers.insert(std::make_pair(key, val));
            return *this;
        }

        http_response_builder &header(string &&key, string &&val) {
            response.headers.insert(std::make_pair(std::move(key), std::move(val)));
            return *this;
        }

        http_response_builder &content(const string &data) {
            response.contents.emplace_back(data);
            return *this;
        }

        http_response_builder &content(string &&data) {
            response.contents.emplace_back(std::move(data));
            return *this;
        }

    private:
        http_response response;

    };

}
#endif //WEBKISH_HTTP_PARSER_H
