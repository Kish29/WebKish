//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_EVENT_LOOPER_H
#define WEBKISH_EVENT_LOOPER_H

#include "base.h"
#include "thread.h"
#include "wakeup_handler.h"

namespace kish {

    typedef std::vector<thread_func> funcs;

    // event_looper是整个服务器的核心，
    // 拥有一个wakeup_epoller，负责不断地向系统询问是否有新的IO事件到来
    class event_looper : copyable, public thread {
    public:

        event_looper();

        ~event_looper();

        event_looper(thread_func func, std::string name);

        void submit(thread_func exe);

        void wakeup() const;

        void add_observe(const shared_ptr<epoll_handler> &eh);

    private:
        epoller poller;
        std::shared_ptr<wakeup_handler> wakeuper;
        funcs pending_funcs;
        mutex_lock locker;

    private:
        void loop();
    };

}


#endif //WEBKISH_EVENT_LOOPER_H
