//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_ACCEPT_HANDLER_H
#define WEBKISH_ACCEPT_HANDLER_H

#include "base.h"
#include "socket.h"
#include "epoll_handler.h"

namespace kish {

    typedef std::function<void(int, const inet_address &)> accept_callback;

    class accept_handler : public epoll_handler, noncopyable {
    public:

        accept_handler(int fd, socket &sk);

        void on_acceptnew(const accept_callback &cb);

        int fd() const override {
            return m_serv_sock.fd();
        }

        void handle_event(uint32_t events) override;

        uint32_t events() const override;

    private:
        socket &m_serv_sock;
        accept_callback m_acc_cb{};
    };
}


#endif //WEBKISH_ACCEPT_HANDLER_H
