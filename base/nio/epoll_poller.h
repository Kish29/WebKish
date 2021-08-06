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
#include "logger.h"
#include "unistd.h"

using namespace kish;
using std::shared_ptr;

namespace kish {

    const int KNONE_EVENT = 0;

    /* ⚠️重要！！！⚠️
     * 按照本框架的响应式模式，每当有一个新的连接到来时，会通过wakeup唤醒epoll_wait阻塞的looper(如果被阻塞的话)
     * 而按照http的请求方式，先是tcp连接事件，然后是http请求，那么，当服务器的socket监听到一个读事件的时候，
     * 使用accept获取client的fd，而此时，该client_fd一定还存在http的请求数据，而此时，epoll将消耗该连接事件
     * 通知服务器，而此时服务器有两种选择：
     * 1. 使用accept获取client_fd，同时再把该client_fd里的http请求数据一次性读完，然后交给looper  --------- 符合epoll的边缘触发方式️
     * 2. 使用accept获取client_fd，然后把该client_fd交给looper，looper再去监听该client_fd的http请求数据 ------- 符合epoll的边缘触发方式️
     * 而本框架采用的是第二种方式的多路复用模型，所以，epoll的事件触发不能采用edge trigger，只能采用水平
     * 两种触发方式几乎无性能差别，只不过edge trigger要求一次IO到来，对应的fd应该把数据都读取完，然后等待下一次的IO事件
     * ⚠️重要！！！⚠️*/
    const int KREAD_EVENT = EPOLLIN | EPOLLPRI;
    const int KWRITE_EVENT = EPOLLOUT;
    const int KERROR_EVENT = EPOLLHUP | EPOLLERR;

    const int KEPOLL_WAITTIME = 10000;  // 10s
//    const int KEPOLL_WAITTIME = -1;  // infinity会导致save_map不能被清理
    const int KINIT_EVENT_SIZE = 16;

    // 核心类
    class epoll_poller : public poller {
        typedef std::vector<epoll_event> event_list;
        typedef std::map<int, handler_ptr> handler_map;
    public:

        epoll_poller();

        ~epoll_poller() override {
#ifdef __DEBUG__
            LOG_INFO << "~epoll_poller() close fd[ " << epoll_fd << "]";
#endif
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
