//
// Created by 蒋澳然 on 2021/8/3.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_THREAD_POOL_H
#define WEBKISH_THREAD_POOL_H

#include "base.h"
#include "thread.h"
#include "vector"

namespace kish {

    const int KMAX_THREADS = 1 << 10;    // 1024
    const int KMAX_QUEUE = (1 << 16) - 1;    // 65536

    enum shutdown_mode {
        IMMEDIATE_SHUTDOWN = 1,
        GRACEFUL_SHUTDOWN = 2
    };

    enum pool_errno {
        POOL_NOERR = 0,
        INVALID = -1,
        LOCK_FAILURE = -2,
        QUEUE_FULL = -3,
        SHUTDOWN = -4,
        THREAD_FAILURE = -5,
        GRACEFUL = -6
    };

    class thread_pool : noncopyable {
    public:
        thread_pool(const thread_pool &) = delete;

        thread_pool &operator=(const thread_pool &) = delete;

    public:

        void init(int thread_num, int queue_size = KDEFAULT_QUEUE_SIZE);

        int submit(const callable &task);

        static thread_pool &instance();

        static int shutdown(shutdown_mode mode = GRACEFUL_SHUTDOWN);

    private:
        static const int KDEFAULT_QUEUE_SIZE;

        mutex_lock m_locker{};
        mutex_cond m_cond{m_locker};

        std::vector<thread_ptr> m_threads{};
        std::vector<callable> m_task_queue{};
        int m_thread_num{CPU_CORE};
        int m_queue_size{KDEFAULT_QUEUE_SIZE};
        int m_curr{0};
        int m_tail{0};
        int m_task_count{0};
        std::atomic_bool m_shutdown{false};
        volatile int m_shutdown_mode{GRACEFUL_SHUTDOWN};

        // cpu集可以认为是一个掩码，每个设置的位都对应一个可以合法调度的 cpu，
        // 而未设置的位则对应一个不可调度的 CPU。换而言之，线程都被绑定了，
        // 只能在那些对应位被设置了的处理器上运行。
        // 通常，掩码中的所有位都被置位了，也就是可以在所有的cpu中调度。
        cpu_set_t m_mask{};

    private:
        thread_pool() = default;

        ~thread_pool();

        int shutdown_internal(shutdown_mode mode);

        void thread_pool_task();

    private:
        static pthread_once_t m_ponce;
        static thread_pool *m_instance;

        static void pool_init();

        static void pool_exit();

    };

}

#define EXECUTOR_POOL kish::thread_pool::instance()

#endif //WEBKISH_THREAD_POOL_H
