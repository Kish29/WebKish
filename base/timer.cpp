//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#include "timer.h"

kish::timer::timer(std::string name)
        : tm_name(std::move(name)),
          task_executor(nullptr),
          locker(),
          cond(locker) {}

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
    if (task_executor) {
        tm_stop = true;
        {
            kish::mutex_lockguard lck(locker);
            cond.notify_all();
        }
        task_executor->join();
    }
}

void kish::timer::schedule(kish::callable &&task, bool async, bool loop, uint32_t ms_t) {
    if (async) {
        // 丢弃之前的任务
        abandon_prevtask();
        task_executor = std::make_shared<kish::thread>([&, task, loop, ms_t]() -> void {
            do {
                {
                    // 获取锁，再wait释放锁
                    kish::mutex_lockguard lck(locker);
                    cond.wait_for_ms(ms_t);
                }
                // 先检查是否以及退出或者任务被丢弃
                if (tm_stop) {
                    break;
                }
                // 执行任务
                task();
            } while (loop);
        }, tm_name + "-task-thread");
        task_executor->start();
    } else {
        do {
            usleep(ms_t * 1000);
            task();
        } while (loop);
    }
}

void kish::timer::abandon_prevtask() {
    stop();
    tm_stop = false; // 重新设置，以便下一个线程执行
}

kish::timer::~timer() {
    stop();
}
