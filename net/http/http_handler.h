//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_HANDLER_H
#define WEBKISH_HTTP_HANDLER_H

#include "tcp_handler.h"
#include "http_interface.h"

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
            // todo: 这个算法好垃圾。。。。
            parameters param;
            param.insert(std::make_pair("fruit", "apple"));
            param.insert(std::make_pair("name", "jar"));
            param.insert(std::make_pair("password", "123456"));
            // todo: 如何查找更快？？？
            // 队头队尾
            size_t len = GLO_RESOLR_MAPPERS.size();
            for (size_t i = 0, j = len - 1; i <= j; i++, j--) {
                if (i == j) {
                    if (GLO_RESOLR_MAPPERS.at(i)->can_resolve("index.html")) {
                        GLO_RESOLR_MAPPERS.at(i)->on_request("index.html", param);
                        break;
                    }
                } else {
                    if (GLO_RESOLR_MAPPERS.at(i)->can_resolve("index.html")) {
                        GLO_RESOLR_MAPPERS.at(i)->on_request("index.html", param);
                        break;
                    }
                    if (GLO_RESOLR_MAPPERS.at(j)->can_resolve("index.html")) {
                        GLO_RESOLR_MAPPERS.at(j)->on_request("index.html", param);
                        break;
                    }
                }

            }
        }

#endif

    private:
        bool m_keep_alive{false};

    private:
        friend bool reg_http_interfc(const http_infc_ptr &);

        static const int KDEFAULT_RESOLVER_SIZE = 2048;

        typedef shared_ptr<http_interface> resolver_ptr;
        // 保存全局注册的方法，解析请求后，调用
        static std::vector<resolver_ptr> GLO_RESOLR_MAPPERS;
    };

}


#endif //WEBKISH_HTTP_HANDLER_H
