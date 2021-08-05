//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "sys/eventfd.h"
#include "event_looper.h"
#include "logger.h"

void event_looper::add_observe(const handler_ptr &eh) {
    if (judge_in_thread()) {
        poller.addev(eh);
    } else {
        mutex_lockguard lck(locker);
        pending_handlers.emplace_back(eh);
        // 唤醒looper
        wakeup();
    }
}

void event_looper::wakeup() const {
    wakeuper->wakeup();
}

event_looper::event_looper()
        : event_looper(std::bind(&event_looper::loop, this), "thread-looper") {}

event_looper::event_looper(std::string name) : event_looper(std::bind(&event_looper::loop, this), std::move(name)) {}

void event_looper::loop() {
    LOG_TRACE << "looper[" << poller.fd() << "] starts loop";
    loop_started = true;
    while (!finished) {
        handler_list list = poller.poll(KEPOLL_WAITTIME);

        // 更新触发的IO事件总数
        total_polled_events += list.size();

        for (handler_ptr &h: list) {
            // poller会将handler的latest_event进行更新
            h->handle_event(h->events());
        }
        handlers hds;
        {
            mutex_lockguard lck(locker);
            hds.swap(pending_handlers);
        }
        for (handler_ptr &f: hds) {
            add_observe(f);
        }
    }
}


event_looper::event_looper(thread_func func, std::string name)
        : thread(std::move(func), std::move(name), true),
          wakeuper(new wakeup_handler(eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC))) {
    if (wakeuper->fd() == -1) {
        // todo log error
    } else {
        // ❌️这样写是不会有计数的！！！！
        // ❌️智能指针教科书式的错误用法
//        std::shared_ptr<epoll_handler> eh(dynamic_cast<epoll_handler *>(wakeuper.get()));
        poller.addev(wakeuper);
    }
}

event_looper::~event_looper() {
    // todo: thread exit safe
//    finished = true;
}

