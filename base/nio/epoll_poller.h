//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_EPOLL_POLLER_H
#define WEBKISH_EPOLL_POLLER_H

#include "sys/epoll.h"
#include "poller.h"
#include "map"
#include "unistd.h"

using namespace kish;
using std::shared_ptr;

namespace kish {

    const int KNONE_EVENT = 0;
    // 均为边缘触发
    const int KREAD_EVENT = EPOLLIN | EPOLLPRI | EPOLLET;
    const int KWRITE_EVENT = EPOLLOUT | EPOLLET;
    const int KERROR_EVENT = EPOLLHUP | EPOLLERR | EPOLLET;
    const int KEPOLL_WAITTIME = 10000;  // 10s
    const int KINIT_EVENT_SIZE = 16;

    // 核心类
    class epoll_poller : public poller {
        typedef std::vector<epoll_event> event_list;
        typedef std::map<int, handler_ptr> handler_map;
    public:

        epoll_poller();

        ~epoll_poller() override {
            // todo: delete this printf
            printf("~epoll_poller() close fd: %d\n", epoll_fd);
            ::close(epoll_fd);
        }

        handler_list &poll(int timeout) override;

        // obs = observable
        void addev(const handler_ptr &obs) override;

        void delev(const handler_ptr &obs) override;

        void modev(const handler_ptr &obs) override;

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

#endif //WEBKISH_EPOLL_POLLER_H