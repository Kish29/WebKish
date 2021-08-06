//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_HANDLER_H
#define WEBKISH_HTTP_HANDLER_H

#include "tcp_handler.h"
#include "http_interface.h"
#include "time_stamp.h"
#include "server_config.h"

namespace kish {

    typedef shared_ptr<http_interface> resolver_ptr;

    class http_handler : public tcp_handler {
        typedef tcp_handler base;
    public:

        explicit http_handler(int fd);

        void handle_read() override;

        bool keep_alive() const;

        bool has_set_timeout() const;

        uint32_t get_timeout() const { return timeout; }

        const time_stamp &get_last_heart_rev_time() const { return latest_heart_rev_time; }

        void set_dead();

    protected:
        void on_req_parse_error();

        void on_req_parse_uri_complete(const http_request_ptr &request);

        parse_control on_req_parse_headers_complete(const http_request_ptr &request);

        void on_message_parse_complete(const http_request_ptr &request);

    protected:

        keep_alive_t alive{KEEP_ALIVE};
        uint32_t timeout{60};   // 60s
        http_request_parser req_parser{};
        // 最新的心跳包接受时间
        time_stamp latest_heart_rev_time{};

        bool has_resolver{false};
        std::vector<resolver_ptr>::iterator resolver{};

    protected:
        friend bool reg_http_interfc(const http_infc_ptr &);

        static const int KDEFAULT_RESOLVER_SIZE = 2048;

        // 保存全局注册的方法，解析请求后，调用
        static std::vector<resolver_ptr> GLO_RESOLR_MAPPERS;
    };

}


#endif //WEBKISH_HTTP_HANDLER_H
