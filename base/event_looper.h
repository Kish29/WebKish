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

    typedef std::vector<handler_ptr> handlers;

    class event_looper;

    class looper_pool;

    // event_looper是整个服务器的核心，
    // 拥有一个wakeup_epoller，负责唤起由于epoll_wait而阻塞的poller
    class event_looper : noncopyable, public thread {
    public:

        event_looper();

        explicit event_looper(std::string name);

        event_looper(thread_func func, std::string name);

        ~event_looper() override;

        void wakeup() const;

        void add_observe(const handler_ptr &eh);

    private:
        epoll_poller poller;
        std::shared_ptr<wakeup_handler> wakeuper;
        handlers pending_handlers;
        mutex_lock locker;

        volatile uint64_t total_polled_events{0};
        // 为防止线程还没有进入，而被外部调用
        std::atomic_bool loop_started{false};

    private:
        void loop();

    private:
        friend class looper_pool;
    };

}


#endif //WEBKISH_EVENT_LOOPER_H
