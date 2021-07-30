//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "epoll_handler.h"

namespace kish {

    void epoll_handler::update_latest_events(uint32_t levents) {
        latest_events = levents;
    }

    void epoll_handler::handle_event(uint32_t events) {
        if (events & KREAD_EVENT) {
            handle_read();
        } else if (events & KWRITE_EVENT) {
            handle_write();
        } else if (events & KERROR_EVENT) {
            handle_error();
        } else {
            // todo: ???
        }
    }

// todo: always call this function
    void epoll_handler::handle_read() {
        // todo: delete this print
        char buf[BUFSIZ];
        // todo: 设置nonblocking，否则会一直zuse
        ssize_t n = read(observe_fd, buf, BUFSIZ);
        printf("new read event! and n is %lu\n", n);
        if (n == 0) {   // 说明客户端已经关闭了
            // 不要调用::close(fd)!!!，close只能在析构函数中调用
            dd = true;
        }
    }

    void epoll_handler::handle_write() {

    }

    void epoll_handler::handle_error() {

    }
}
