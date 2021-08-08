//
// Created by 蒋澳然 on 2021/8/3.
// email: 875691208@qq.com
// $desc
//

#include "thread_pool.h"

#include "kish_utils.h"
#include "logger.h"

const int kish::thread_pool::KDEFAULT_QUEUE_SIZE = 2048;

kish::thread_pool::thread_pool(int thread_num, int queue_size) {
    // 参数合法化
    thr_num = coerce_in(thread_num, 1, KMAX_THREADS);
    que_siz = coerce_in(queue_size, KDEFAULT_QUEUE_SIZE, KMAX_QUEUE);

    threads.reserve(thr_num);
    // 注意task_queue是resize，即直接分配了内存
    task_queue.resize(que_siz);

    curr = tail = task_count = 0;

    for (int i = 0; i < thr_num; ++i) {
        threads.emplace_back(std::make_shared<thread>(
                std::bind(&thread_pool::thread_pool_task, this),
                "thread-pool-executor-" + std::to_string(i)));

    }

    for (const thread_ptr &p: threads) {
        if (p) {
            p->start();
        }
    }

    LOG_INFO << "thread-pool inited, thead num: " << thr_num << " queue size :" << queue_size;
}

kish::thread_pool::~thread_pool() {
    shutdown_internal(IMMEDIATE_SHUTDOWN);
}


int kish::thread_pool::shutdown(kish::shutdown_mode mode) {
    return shutdown_internal(mode);
}

int kish::thread_pool::shutdown_internal(kish::shutdown_mode mode) {
    {
        mutex_lockguard lck(locker);
        if (shutdn) {
            return SHUTDOWN;
        }
        // 设置shutdown模式
        shutdn_mode = mode;
        shutdn = true;
        // 通知所有线程shutdown
        cond.notify_all();
    }

    for (const thread_ptr &p: threads) {
        if (p->join() != 0) {
            return THREAD_FAILURE;
        }
    }

    return POOL_NOERR;
}

int kish::thread_pool::submit(const kish::callable &task) {
    {
        mutex_lockguard lck(locker);
        // 工作队列已满
        if (task_count == que_siz) {
            return QUEUE_FULL;
        }
        // 线程池已经关闭
        if (shutdn) {
            return SHUTDOWN;
        }
        task_queue.at(tail) = task;

        // 更新下一个executor
        tail++;
        tail %= que_siz;

        task_count++;

        // 唤醒一个线程
        cond.notify_one();
    }
    return 0;
}

void kish::thread_pool::thread_pool_task() {
    while (true) {
        callable task;
        {
            mutex_lockguard lck(locker);
            while (!shutdn && task_count == 0) {
                cond.wait();
            }
            if (shutdn) {
                switch (shutdn_mode) {
                    // todo: switch shutdown mode
                }
                break;
            }

            // 取出任务
            task = std::move(task_queue.at(curr));
            curr++;
            curr %= que_siz;
            task_count--;
        }

        // execute
        task();
    }
    pthread_exit(nullptr);
}

