//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_PARSER_H
#define WEBKISH_HTTP_PARSER_H

#include "base.h"
#include "http_request.h"
#include "http_response.h"

const char *get_file_mime_type(const char *filename);

namespace kish {

    enum parse_control {
        GO_ON,      // 表示调用者检测通过，继续解析
        SKIP,       // 表示调用者跳过此报文的解析    --->  调用message_complete
        ERROR       // 表示调用者检测失败，parser内部会重置所有的状态和资源
    };

    class request_multipart_parser : noncopyable {
        enum last_part_state_t {
            IDLE,
            LAST_FIELD,
            LAST_VALUE
        };
    public:
        explicit request_multipart_parser(const std::string &boundary) : request_multipart_parser(boundary.c_str()) {}

        explicit request_multipart_parser(const char *boundary) {
            bzero(&settings, sizeof settings);
            settings.on_header_value = mp_on_read_header_value;
            settings.on_part_data = mp_on_part_data;
            m_parser = multipart_parser_init(boundary, &settings);
            multipart_parser_set_data(m_parser, this);
        }

        ~request_multipart_parser() override {
            multipart_parser_free(m_parser);
        }

        param_container &parse_multipart(const char *body, size_t bodylen);

        param_container &parse_multipart(const std::string &body) {
            return parse_multipart(body.c_str(), body.size());
        }

    protected:
#define MP reinterpret_cast<request_multipart_parser *>(multipart_parser_get_data(parser))

        static int mp_on_read_header_value(multipart_parser *parser, const char *at, size_t len) { return MP->on_read_header_value(parser, at, len); }

        static int mp_on_part_data(multipart_parser *parser, const char *at, size_t len) { return MP->on_part_data(parser, at, len); }

#undef MP

        int on_read_header_value(multipart_parser *parser, const char *at, size_t len);

        int on_part_data(multipart_parser *parser, const char *at, size_t len);

    protected:
        multipart_parser *m_parser;
        multipart_parser_settings settings{};

        last_part_state_t last_s{IDLE};

        std::string field{};
        std::string value{};

        param_container params;

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

}
#endif //WEBKISH_HTTP_PARSER_H
