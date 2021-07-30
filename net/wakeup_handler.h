//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_WAKEUP_HANDLER_H
#define WEBKISH_WAKEUP_HANDLER_H

#include "epoll_handler.h"

namespace kish {

    class wakeup_handler : public epoll_handler {
    public:

        explicit wakeup_handler(int fd) : epoll_handler(fd) {}

        // poll如果在timeout>0的情况下，没有io事件，会阻塞住调用poll的线程
        // 如果要立即唤醒poller，请在另一个线程中调用wakeup
        void wakeup() const;

        void handle_read() override;

        int fd() const override {
            return observe_fd;
        }

        uint32_t events() const override {
            return KREAD_EVENT;
        }

    };

}


#endif //WEBKISH_WAKEUP_HANDLER_H
