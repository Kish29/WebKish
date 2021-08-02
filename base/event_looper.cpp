//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "event_looper.h"
#include "sys/eventfd.h"

void event_looper::submit(thread_func exe) {
    char buf[64];
    pthread_getname_np(pthread_self(), buf, 64);
    // todo: delete this printf
    printf("submit in %s\n", buf);
    if (judge_in_thread()) {
        exe();
    } else {
        mutex_lockguard lck(m_locker);
        m_pending_funcs.emplace_back(std::move(exe));
        // 唤醒looper
        wakeup();
    }
}

void event_looper::wakeup() const {
    // todo: delete this printf
    printf("event_looper wakeup\n");
    char buf[64];
    pthread_getname_np(pthread_self(), buf, 64);
    // todo: delete this printf
    printf("m_poller.wakeup in %s\n", buf);
    m_wakeuper->wakeup();
}

event_looper::event_looper()
        : event_looper(std::bind(&event_looper::loop, this), "thread-m_looper") {}


void event_looper::loop() {
    // todo: delete this printf
    // todo: here printed "no"
    printf("run in thread ? %s\n", judge_in_thread() ? "yes" : "no");
    while (!m_finished) {
        // todo: delete this printf
        printf("event_looper do poll\n");
        handler_list list = m_poller.poll(KEPOLL_WAITTIME);
        for (handler_ptr &h: list) {
            // poller会将handler的latest_event进行更新
            h->handle_event(h->events());
        }
        // todo: doing pending functors
        printf("do pending functors\n");
        std::vector<thread_func> functors;
        functors.swap(m_pending_funcs);
        for (thread_func &f:functors) {
            f();
        }
    }
}

void event_looper::add_observe(const handler_ptr &eh) {
    m_poller.addev(eh);
}

event_looper::event_looper(thread_func func, std::string name)
        : thread(std::move(func), std::move(name), true),
          m_wakeuper(new wakeup_handler(eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC))) {
    if (m_wakeuper->fd() == -1) {
        // todo log error
    } else {
        // ❌️这样写是不会有计数的！！！！
        // ❌️智能指针教科书式的错误用法
//        std::shared_ptr<epoll_handler> eh(dynamic_cast<epoll_handler *>(m_wakeuper.get()));
        m_poller.addev(m_wakeuper);
    }
}

event_looper::~event_looper() {
    // todo: thread exit safe
//    m_finished = true;
}
