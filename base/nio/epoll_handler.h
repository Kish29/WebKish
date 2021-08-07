//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_EPOLL_HANDLER_H
#define WEBKISH_EPOLL_HANDLER_H

#include "epoll_poller.h"
#include "event_handler.h"

namespace kish {

    constexpr int KREAD_BUFSIZ = 2 * 1024;
    constexpr int KSEND_BUFSIZ = 8 * 1024;

    // 核心类，代表可被poller进行观察和事件处理的对象
    class epoll_handler : public event_handler {
    public:

        explicit epoll_handler(int fd)
                : observe_fd(fd),
                  observe_events(KREAD_EVENT | KWRITE_EVENT | KERROR_EVENT) {}

        /* ⚠️警告Warning⚠️️原则上禁止子类调用close将fd关闭掉，close只能在基类的析构函数中进行 ⚠️*/
        ~epoll_handler() override {
            if (!fd_closed) {
#ifdef __DEBUG__
                LOG_INFO << "epoll_handler() close fd[" << observe_fd << "]";
#endif
                ::close(observe_fd);
            }
        }

        int fd() const override {
            return observe_fd;
        }

        uint32_t events() const override {
            return observe_events;
        }

        void update_latest_events(uint32_t levents) override;

        void handle_event(uint32_t events) override;

        virtual void handle_read();

        virtual void handle_write();

        virtual void handle_error();

        // 判断是否供给poller使用，以便从save_map中移除
        // 取名为dead的原因是close单词冲突了
        bool dead() const override {
            return i_am_dead;
        };

        virtual ssize_t readin() final;

    protected:
        int observe_fd;
        // observe_events将会被添加进poller中，作为观察的事件
        uint32_t observe_events;
        // latest_events将会被poller更新，以便handler自己在
        // handle_event中进行事件判断
        uint32_t latest_events{KNONE_EVENT};

        bool fd_closed{false};
        bool i_am_dead{false};
        // 由于IO事件到来后必须从缓冲区读取数据
        // 否则epoll_wait始终有事件
        char read_buf[KREAD_BUFSIZ]{};
        ssize_t curr_read_len{0};
    };
}


#endif //WEBKISH_EPOLL_HANDLER_H
