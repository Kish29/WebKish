//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_TCP_SERVER_H
#define WEBKISH_TCP_SERVER_H

#include "base.h"
#include "socket.h"
#include "server.h"
#include "event_looper.h"

namespace kish {

    class tcp_server : public server {
    public:
        explicit tcp_server(uint16_t port);

        tcp_server(uint16_t port, const string &host);

        void startup() override;

        void stop() override;

        void on_acceptnew(int, const inet_address &) override;

    private:
        event_looper looper;
        socket server_sock;
        inet_address server_addr;
        shared_ptr<acceptor> accep;
        bool started{};
        bool stopped{};
    };

}


#endif //WEBKISH_TCP_SERVER_H
