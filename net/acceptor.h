//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_ACCEPTOR_H
#define WEBKISH_ACCEPTOR_H

#include "base.h"
#include "socket.h"
#include "epoll_handler.h"

namespace kish {

    typedef std::function<void(int, const inet_address &)> accept_callback;

    class acceptor : public epoll_handler, noncopyable {
    public:

        acceptor(int fd, socket &sk);

        void on_acceptnew(const accept_callback &cb);

        int fd() const override {
            return server_sock.fd();
        }

        void handle_event(uint32_t events) override;

        uint32_t events() const override;

    private:
        socket &server_sock;
        accept_callback onaccept{};
    };
}


#endif //WEBKISH_ACCEPTOR_H
