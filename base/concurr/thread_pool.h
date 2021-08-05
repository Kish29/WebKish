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
        explicit thread_pool(int thread_num, int queue_size = KDEFAULT_QUEUE_SIZE);

        ~thread_pool() override;

        int submit(const callable &task);

        int shutdown(shutdown_mode mode = GRACEFUL_SHUTDOWN);

    private:
        static const int KDEFAULT_QUEUE_SIZE;

        mutex_lock locker{};
        mutex_cond cond{locker};

        std::vector<thread_ptr> threads{};
        std::vector<callable> task_queue{};
        int thr_num{CPU_CORE};
        int que_siz{KDEFAULT_QUEUE_SIZE};
        int curr{0};
        int tail{0};
        int task_count{0};
        std::atomic_bool shutdn{false};
        volatile int shutdn_mode{GRACEFUL_SHUTDOWN};

    private:

        int shutdown_internal(shutdown_mode mode);

        void thread_pool_task();

    };

}

#endif //WEBKISH_THREAD_POOL_H
