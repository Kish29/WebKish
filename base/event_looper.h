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
    // 拥有一个wakeup_epoller，负责唤起由于epoll_wait而阻塞的poller
    class event_looper : copyable, public thread {
    public:

        event_looper();

        explicit event_looper(std::string name);

        event_looper(thread_func func, std::string name);

        ~event_looper();

        void submit(thread_func exe);

        void wakeup() const;

        void add_observe(const handler_ptr &eh);

    private:
        epoll_poller m_poller;
        std::shared_ptr<wakeup_handler> m_wakeuper;
        funcs m_pending_funcs;
        mutex_lock m_locker;

    private:
        void loop();
    };

}


#endif //WEBKISH_EVENT_LOOPER_H
