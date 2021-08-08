//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_SERVER_H
#define WEBKISH_HTTP_SERVER_H

#include "http_handler.h"
#include "tcp_server.h"
#include "timer.h"
#include "http_heart_check.h"
#include "server_config.h"

namespace kish {

    class http_server : public tcp_server, public http_heart_check {
        typedef tcp_server base;
    public:

        explicit http_server(uint16_t port) : tcp_server(port) {
            connectors.reserve(KISH_CONFIG.MAX_SERVER_CNN());
        }

        http_server(uint16_t port, const string &host, int looper_num = CPU_CORE) : tcp_server(port, host, looper_num) {}

        http_server(uint16_t port, int looper_num, const string &host = "0.0.0.0") : tcp_server(port, host, looper_num) {}

        void on_acceptnew(int fd, const inet_address &peer_addr) override;

        void check_for_alive() override;

        void startup() override;

    private:
        typedef std::shared_ptr<http_handler> http_handler_ptr;
        timer serv_timer{"http-server timer"};
        std::vector<http_handler_ptr> connectors{};

        mutex_lock locker;
    };
}


#endif //WEBKISH_HTTP_SERVER_H
