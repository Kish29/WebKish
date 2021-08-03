//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#include "timer.h"

kish::timer::timer(std::string name)
        : m_name(std::move(name)),
          m_exe(nullptr),
          m_locker(),
          m_cond(m_locker) {}

void kish::timer::async(kish::callable &&task, uint32_t ms_t) {
    schedule(std::move(task), true, false, ms_t);
}

void kish::timer::async_loop(kish::callable &&task, uint32_t ms_t) {
    schedule(std::move(task), true, true, ms_t);
}

void kish::timer::sync(kish::callable &&task, uint32_t ms_t) {
    schedule(std::move(task), false, false, ms_t);
}

void kish::timer::sync_loop(kish::callable &&task, uint32_t ms_t) {
    schedule(std::move(task), false, true, ms_t);
}

void kish::timer::stop() {
    if (m_exe) {
        m_stop = true;
        {
            kish::mutex_lockguard lck(m_locker);
            m_cond.notify_all();
        }
        m_exe->join();
    }
}

void kish::timer::schedule(kish::callable &&task, bool async, bool loop, uint32_t ms_t) {
    if (async) {
        // 丢弃之前的任务
        abandon_prevtask();
        m_exe = std::make_shared<kish::thread>([&, task, loop, ms_t]() -> void {
            do {
                {
                    // 获取锁，再wait释放锁
                    kish::mutex_lockguard lck(m_locker);
                    m_cond.wait_for_ms(ms_t);
                }
                // 先检查是否以及退出或者任务被丢弃
                if (m_stop) {
                    break;
                }
                // 执行任务
                task();
            } while (loop);
        }, m_name + "-task-thread");
        m_exe->start();
    } else {
        do {
            usleep(ms_t * 1000);
            task();
        } while (loop);
    }
}

void kish::timer::abandon_prevtask() {
    stop();
    m_stop = false; // 重新设置，以便下一个线程执行
}

kish::timer::~timer() {
    stop();
}
