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

    typedef shared_ptr<http_resolver> resolver_ptr;

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
        // 因为我们的框架设计是用一个主线程进行心跳清理
        // 所以不像其他web服务框架，我们默认保持连接，后续的http请求报文到来时，更新timeout或者keepalive或者close
        keep_alive_t alive{KEEP_ALIVE};
        uint32_t timeout{60};   // 60s
        http_request_parser req_parser{};
        // 最新的心跳包接受时间
        time_stamp latest_heart_rev_time{};

        // 是否有对应的接受者，这里指接口或者resolver
        bool has_resolver{false};
        bool has_hi_func{false};
        // 错误的请求
        bool bad_request{false};
        std::vector<resolver_ptr>::iterator curr_resolver{};
        hi_func curr_hi_func{};

    protected:
        // 注册接口
        friend bool reg_http_interface(const http_intc_ptr &);

        // 注册resolver类
        friend bool reg_http_resolver(const http_resol_ptr &);

        // big enough to use!!!
        static const int KDEFAULT_RESOLVER_SIZE = 512;

        // 保存全局注册的方法，解析请求后，调用
        static std::vector<resolver_ptr> GLO_RESOLR_MAPPERS;

        static http_interface_holder GLO_INTFC_HOLDER;

        static void default_404_content(http_response &response);

        static void default_400_content(http_response &response);

    };

}


#endif //WEBKISH_HTTP_HANDLER_H
