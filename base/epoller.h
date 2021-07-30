//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_EPOLLER_H
#define WEBKISH_EPOLLER_H

#include "vector"
#include "sys/epoll.h"
#include "fdholder.h"
#include "memory"
#include "map"

using namespace kish;
using std::shared_ptr;

namespace kish {
    class epoll_handler;

    class epoller;
}

typedef std::vector<epoll_event> event_list;
typedef std::vector<shared_ptr<epoll_handler>> handler_list;
typedef std::map<int, shared_ptr<epoll_handler>> handler_map;

#include "cstdio"

namespace kish {

    const int KNONE_EVENT = 0;
    // 均为边缘触发
    const int KREAD_EVENT = EPOLLIN | EPOLLPRI | EPOLLET;
    const int KWRITE_EVENT = EPOLLOUT | EPOLLET;
    const int KERROR_EVENT = EPOLLHUP | EPOLLERR | EPOLLET;
    const int KEPOLL_WAITTIME = 10000;  // 10s
    const int KINIT_EVENT_SIZE = 16;  // 10s

    // 核心类
    class epoller : fdholder {
    public:

        epoller();

        ~epoller() override {
            // todo: delete this printf
            printf("~epoller() close fd: %d\n", epoll_fd);
            close(epoll_fd);
        }

        handler_list &poll(int timeout = KEPOLL_WAITTIME);

        // obs = observable
        void epoll_add(const shared_ptr<epoll_handler> &obs);

        void epoll_del(const shared_ptr<epoll_handler> &obs);

        void epoll_mod(const shared_ptr<epoll_handler> &obs);

        int fd() const override {
            return epoll_fd;
        }

    private:
        int epoll_fd;
        event_list query_list;
        handler_map save_map{};
        handler_list ret_list;

    private:
        void update_savemap();

    };

}

#endif //WEBKISH_EPOLLER_H