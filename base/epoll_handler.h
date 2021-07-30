//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_EPOLL_HANDLER_H
#define WEBKISH_EPOLL_HANDLER_H

#include "epoller.h"

namespace kish {

    // 核心类，代表可被poller进行观察和事件处理的对象
    class epoll_handler : fdholder, event_handler {
    public:

        explicit epoll_handler(int fd)
                : observe_fd(fd),
                  observe_events(KREAD_EVENT | KWRITE_EVENT | KERROR_EVENT) {}

        /* ⚠️警告Warning⚠️️原则上禁止子类调用close将fd关闭掉，close只能在基类的析构函数中进行 ⚠️*/
        ~epoll_handler() override {
            if (!clsd) {
                // todo: verify what will happen when fd is -1 to be closed
                ::close(observe_fd);
            }
        }

        int fd() const override {
            return observe_fd;
        }

        virtual uint32_t events() const {
            return observe_events;
        }

        void update_latest_events(uint32_t levents);

        void handle_event(uint32_t events) override;

        virtual void handle_read();

        virtual void handle_write();

        virtual void handle_error();

        // 判断是否供给poller使用，以便从save_map中移除
        bool dead() const override {
            return dd;
        };

        bool closed() const override {
            return clsd;
        }

    protected:
        int observe_fd;
        // observe_events将会被添加进poller中，作为观察的事件
        uint32_t observe_events;
        // latest_events将会被poller更新，以便handler自己在
        // handle_event中进行事件判断
        uint32_t latest_events{KNOMAL_EVENT};
        bool clsd{false};
        bool dd{false};
    };
}


#endif //WEBKISH_EPOLL_HANDLER_H
