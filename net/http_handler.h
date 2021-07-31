//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_HANDLER_H
#define WEBKISH_HTTP_HANDLER_H

#include "tcp_handler.h"
#include "http_interface.h"
#include "map"

namespace kish {

    class http_handler : public tcp_handler {
        typedef tcp_handler base;
    public:

        explicit http_handler(int fd) : tcp_handler(fd) {}

        void handle_read() override;

        bool keep_alive() const;

        void set_dead();

#ifdef __DEBUG__

        static void test_mappers() {
            for (auto &m:GLOBAL_HTTP_REQUEST_RESOLVERS) {
                request_param rp{};
                rp.insert(std::make_pair("name", "jar"));
                rp.insert(std::make_pair("birthday", "2021-8-1"));
                rp.insert(std::make_pair("fruit", "apple"));
                m.second->on_request(rp);
            }
        }

#endif

    private:
        bool kp_alv{false};

        typedef std::string url;
        typedef shared_ptr<http_interface> resolver_ptr;

        // 保存全局注册的方法，解析请求后，调用
        static std::map<url, resolver_ptr> GLOBAL_HTTP_REQUEST_RESOLVERS;
    private:

        friend bool reg_http_resolver(const http_infc_ptr &);
    };

}


#endif //WEBKISH_HTTP_HANDLER_H
