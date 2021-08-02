//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_SERVER_H
#define WEBKISH_HTTP_SERVER_H

#include "tcp_server.h"

namespace kish {

    class http_server : public tcp_server {
    public:

        explicit http_server(uint16_t port) : tcp_server(port) {}

        http_server(uint16_t port, const string &host) : tcp_server(port, host) {}

        void on_acceptnew(int fd, const inet_address &peer_addr) override;

    };
}


#endif //WEBKISH_HTTP_SERVER_H
