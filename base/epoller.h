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

    const int KNOMAL_EVENT = 0;
    const int KREAD_EVENT = EPOLLIN | EPOLLPRI;
    const int KWRITE_EVENT = EPOLLOUT;
    const int KERROR_EVENT = EPOLLHUP | EPOLLERR;
    const int KEPOLL_WAITTIME = 10000;  // 10s
    const int KINIT_EVENT_SIZE = 16;  // 10s

    // 核心类
    class epoller : fdholder {
    public:

        epoller();

        ~epoller() override {
            char buf[64];
            pthread_getname_np(pthread_self(), buf, 64);
            // todo: delete this printf
            printf("~epoller() in %s\n", buf);
            close(epoll_fd);
        }

        handler_list &poll(int timeout = KEPOLL_WAITTIME);

        void epoll_add(const shared_ptr<epoll_handler> &eh);

        void epoll_del(shared_ptr<fdholder> h);

        void epoll_mod(shared_ptr<fdholder> h);

        int fd() const override {
            return epoll_fd;
        }

        virtual void update_savemap() final;

    protected:
        int epoll_fd;
        event_list epoll_evlst;
        handler_map save_map{};
        handler_list retlist;
    };

}

#endif //WEBKISH_EPOLLER_H