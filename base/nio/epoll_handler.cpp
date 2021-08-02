//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "epoll_handler.h"
#include "sysio_utils.h"

namespace kish {

    void epoll_handler::update_latest_events(uint32_t levents) {
        m_latest_events = levents;
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

    // 基本实现，只负责对该文件描述符做数据读取
    void epoll_handler::handle_read() {
        ssize_t n = readin();
        if (n < 0) {
            m_dead = true;
        }
//        if (rdn < 0) {
        // todo: log error
//            perror("void epoll_handler::handle_read() read n <=0 ");
        // todo: 待商榷 ⚠️基类不应该设置i_am_dead，应当交给网络协议tcp/http
//            m_dead = true;
//        }
        // todo: 如何判断一个文件描述符的错误
//        m_dead = JUDGE_DEAD(n);
    }

    void epoll_handler::handle_write() {

    }

    void epoll_handler::handle_error() {

    }

    ssize_t epoll_handler::readin() {
        bzero(m_read_buf, KREAD_BUFSIZ);
        return read(m_observe_fd, m_read_buf, KREAD_BUFSIZ);
    }
}
