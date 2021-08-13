//
// Created by 蒋澳然 on 2021/8/13.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_RESPONSE_H
#define WEBKISH_HTTP_RESPONSE_H

#include "http_message.h"

namespace kish {

    class http_response : public http_message {
    public:
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


#endif //WEBKISH_HTTP_RESPONSE_H
