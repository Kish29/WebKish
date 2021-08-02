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

    protected:
        event_looper m_looper;
        socket m_serv_sock;
        inet_address m_serv_addr;
        shared_ptr<accept_handler> m_acceptor;
        bool m_started{};
        bool m_stopped{};
    };

}


#endif //WEBKISH_TCP_SERVER_H
