//
// Created by 蒋澳然 on 2021/8/3.
// email: 875691208@qq.com
// $desc
//

#include "thread_pool.h"

#include "util.h"
#include "logger.h"
#include "cassert"

pthread_once_t kish::thread_pool::m_ponce = PTHREAD_ONCE_INIT;
kish::thread_pool *kish::thread_pool::m_instance = nullptr;
const int kish::thread_pool::KDEFAULT_QUEUE_SIZE = 2048;

void kish::thread_pool::pool_init() {
    m_instance = new thread_pool;
    ::atexit(pool_exit);
}

void kish::thread_pool::pool_exit() {
    delete m_instance;
    m_instance = nullptr;
}

kish::thread_pool &kish::thread_pool::instance() {
    if (m_instance == nullptr) {
        pthread_once(&m_ponce, pool_init);
    }
    assert(m_instance);
    return *m_instance;
}

void kish::thread_pool::init(int thread_num, int queue_size) {
    // 参数合法化
    m_thread_num = coerce_in(thread_num, CPU_CORE, KMAX_THREADS);
    m_queue_size = coerce_in(queue_size, KDEFAULT_QUEUE_SIZE, KMAX_QUEUE);

    m_threads.reserve(m_thread_num);
    // 注意task_queue是resize，即直接分配了内存
    m_task_queue.resize(m_queue_size);

    m_curr = m_tail = m_task_count = 0;

    for (int i = 0; i < thread_num; ++i) {
        m_threads.emplace_back(std::make_shared<thread>(
                std::bind(&thread_pool::thread_pool_task, this),
                "thread-pool-executor-" + std::to_string(i)));

    }

    for (const thread_ptr &p: m_threads) {
        if (p) {
            p->start();
        }
    }

    LOG_INFO << "thread-pool inited, thead num: " << m_thread_num << " queue size :" << m_queue_size;
}

kish::thread_pool::~thread_pool() {
    shutdown_internal(IMMEDIATE_SHUTDOWN);
}


int kish::thread_pool::shutdown(kish::shutdown_mode mode) {
    if (m_instance) {
        return m_instance->shutdown_internal(mode);
    }
    return kish::POOL_NOERR;
}

int kish::thread_pool::shutdown_internal(kish::shutdown_mode mode) {
    {
        mutex_lockguard lck(m_locker);
        if (m_shutdown) {
            return SHUTDOWN;
        }
        // 设置shutdown模式
        m_shutdown_mode = mode;
        m_shutdown = true;
        // 通知所有线程shutdown
        m_cond.notify_all();
    }

    for (const thread_ptr &p: m_threads) {
        if (p->join() != 0) {
            return THREAD_FAILURE;
        }
    }

    return POOL_NOERR;
}

int kish::thread_pool::submit(const kish::callable &task) {
    {
        mutex_lockguard lck(m_locker);
        // 工作队列已满
        if (m_task_count == m_queue_size) {
            return QUEUE_FULL;
        }
        // 线程池已经关闭
        if (m_shutdown) {
            return SHUTDOWN;
        }
        m_task_queue.at(m_tail) = task;

        // 更新下一个executor
        m_tail++;
        m_tail %= m_queue_size;

        m_task_count++;

        // 唤醒一个线程
        m_cond.notify_one();
    }
    return 0;
}

void kish::thread_pool::thread_pool_task() {
    while (true) {
        callable task;
        {
            mutex_lockguard lck(m_locker);
            while (!m_shutdown && m_task_count == 0) {
                m_cond.wait();
            }
            if (m_shutdown) {
                // todo: switch shutdown mode
                break;
            }

            // 取出任务
            task = std::move(m_task_queue.at(m_curr));
            m_curr++;
            m_curr %= m_queue_size;
            m_task_count--;
        }

        // execute
        task();
    }
    pthread_exit(nullptr);
}

