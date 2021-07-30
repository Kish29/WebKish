//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#include "acceptor.h"

namespace kish {

    acceptor::acceptor(int fd, socket &sk) : epoll_handler(fd), server_sock(sk) {}

    void acceptor::on_acceptnew(const accept_callback &cb) {
        onaccept = cb;
    }

    void acceptor::handle_event(uint32_t events) {
        if (events & KREAD_EVENT) {
            struct sockaddr_in client_addr{};
            socklen_t claddr_len = sizeof client_addr;
            int client_fd = ::accept(observe_fd, (sockaddr *) &client_addr, &claddr_len);
            // todo: 限制连接数量
            if (client_fd > 0 && onaccept) {
                inet_address a(client_addr.sin_port, client_addr.sin_addr.s_addr, true);
                onaccept(client_fd, a);
            } else {
                ::close(client_fd);
            }
        } else if (events & KERROR_EVENT) {
            // todo: log error
        } else {
            // todo: unknown?
        }
    }

    uint32_t acceptor::events() const {
        return KREAD_EVENT | KERROR_EVENT;
    }
}


