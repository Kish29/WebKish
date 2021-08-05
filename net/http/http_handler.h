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

namespace kish {

    class http_handler : public tcp_handler {
        typedef tcp_handler base;
    public:

        explicit http_handler(int fd) : tcp_handler(fd) {}

        void handle_read() override;

        bool keep_alive() const;

        bool has_set_timeout() const;

        uint32_t get_timeout() const { return timeout; }

        const time_stamp &get_last_heart_rev_time() const { return latest_heart_rev_time; }

        void set_dead();

    private:
        keep_alive_t alive{KEEP_ALIVE};
        uint32_t timeout{60};   // 60s
        http_parser req_parser{http_parser::REQUEST};
        // 最新的心跳包接受时间
        time_stamp latest_heart_rev_time{};

        // 保存不完整的请求，直到完整
        string save_request{};

    private:
        friend bool reg_http_interfc(const http_infc_ptr &);

        static const int KDEFAULT_RESOLVER_SIZE = 2048;

        typedef shared_ptr<http_interface> resolver_ptr;
        // 保存全局注册的方法，解析请求后，调用
        static std::vector<resolver_ptr> GLO_RESOLR_MAPPERS;
    };

}


#endif //WEBKISH_HTTP_HANDLER_H
