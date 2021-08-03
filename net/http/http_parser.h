//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_PARSER_H
#define WEBKISH_HTTP_PARSER_H

#include "base.h"
#include "map"

namespace kish {
    using std::string;

    enum req_method {
        GET,    // GET方法请求一个指定资源的表示形式. 使用GET的请求应该只被用于获取数据.
        HEAD,   // HEAD方法请求一个与GET请求的响应相同的响应，但没有响应体.
        POST,   // POST方法用于将实体提交到指定的资源，通常导致在服务器上的状态变化或副作用.
        PUT,    // PUT方法用请求有效载荷替换目标资源的所有当前表示。会调用数据库update操作
        DELETE, // DELETE方法删除指定的资源。
        CONNECT,    // CONNECT方法建立一个到由目标资源标识的服务器的隧道。
        OPTIONS,  // OPTIONS方法用于描述目标资源的通信选项。
        TRACE,  // TRACE方法沿着到目标资源的路径执行一个消息环回测试。
        PATCH   // PATCH方法用于对资源应用部分修改。
    };

    enum content_type {
        TEXT_PLAIN,

    };

    enum http_version {
        HTTP010,
        HTTP011,
        HTTP020
    };

    struct http_message : copyable, public printable, public jsonable, public message_type {
        typedef std::map<std::string, std::string> header_item;

        http_version version;
        header_item headers;
        std::string content;
        content_type content_t;
        bool keep_alive;

        http_message() : version(HTTP011), headers(), content(), content_t(TEXT_PLAIN), keep_alive(false) {};
    };

    struct http_request : public http_message {
        req_method method;
        std::string uri;

        http_request() : http_message() {
            method = GET;
            uri = "/";
        }

        http_request(req_method meth, string u) : method(meth), uri(std::move(u)) {}

        std::string tostring() const override;

        std::string tojson() const override;

        std::string tomessage() const override;
    };

    struct http_response : public http_message {
        int status_code;
        std::string short_msg;

        http_response() : http_message() {
            status_code = 404;
            // todo: response code map
            short_msg = response_code_map.at(404);
        }

        http_response(int code, string short_msg) : status_code(code), short_msg(std::move(short_msg)) {}

        std::string tostring() const override;

        std::string tojson() const override;

        std::string tomessage() const override;
    };

    // version + code + msg
    // headers
    // content
    struct simp_resp {
        int status{400};
        // todo: body可被序列化 RPC
        string body{};
        content_type type{TEXT_PLAIN};

        simp_resp(int st, string bd, content_type tp = TEXT_PLAIN) : status(st), body(std::move(bd)), type(tp) {}
    };

    class http_parser : noncopyable {
    public:

        static bool parse_req(http_request &req, const char *begin, const char *end);

        static bool parse_resp(http_response &resp, const char *begin, const char *end);
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

        http_request_builder &method(const req_method &mtd) {
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

        http_request_builder &version(const http_version &ver) {
            request.version = ver;
            return *this;
        }

        http_request_builder &keep_alive(bool keep) {
            request.keep_alive = keep;
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
            request.content = data;
            return *this;
        }

        http_request_builder &content(string &&data) {
            request.content = std::move(data);
            return *this;
        }

        http_request_builder &content_type(const content_type &type) {
            request.content_t = type;
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

        http_response_builder &status_code(int code) {
            response.status_code = code;
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

        http_response_builder &version(const http_version &ver) {
            response.version = ver;
            return *this;
        }

        http_response_builder &keep_alive(bool keep) {
            response.keep_alive = keep;
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
            response.content = data;
            return *this;
        }

        http_response_builder &content(string &&data) {
            response.content = std::move(data);
            return *this;
        }

        http_response_builder &content_type(const content_type &type) {
            response.content_t = type;
            return *this;
        }

    private:
        http_response response;

    };

}


#endif //WEBKISH_HTTP_PARSER_H
