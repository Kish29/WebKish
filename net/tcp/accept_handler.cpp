//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#include "accept_handler.h"

namespace kish {

    accept_handler::accept_handler(int fd, socket &sk) : epoll_handler(fd), m_serv_sock(sk) {}

    void accept_handler::on_acceptnew(const accept_callback &cb) {
        m_acc_cb = cb;
    }

    void accept_handler::handle_event(uint32_t events) {
        if (events & KREAD_EVENT) {
            struct sockaddr_in client_addr{};
            socklen_t claddr_len = sizeof client_addr;
            int client_fd = ::accept(m_observe_fd, (sockaddr *) &client_addr, &claddr_len);
            // todo: 限制连接数量
            if (client_fd > 0 && m_acc_cb) {
                // todo: delete this print
                printf("client fd is %d\n", client_fd);
                inet_address a(client_addr.sin_port, client_addr.sin_addr.s_addr, true);
                m_acc_cb(client_fd, a);
            } else {
                ::close(client_fd);
            }
        } else if (events & KERROR_EVENT) {
            // todo: log error
        } else {
            // todo: unknown?
        }
    }

    uint32_t accept_handler::events() const {
        return KREAD_EVENT | KERROR_EVENT;
    }
}


