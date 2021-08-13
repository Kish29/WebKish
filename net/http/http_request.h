//
// Created by 蒋澳然 on 2021/8/13.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_REQUEST_H
#define WEBKISH_HTTP_REQUEST_H

#include "http_message.h"

namespace kish {

    class http_request : public http_message {
    public:
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

}
#endif //WEBKISH_HTTP_REQUEST_H
