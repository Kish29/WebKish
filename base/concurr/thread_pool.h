//
// Created by 蒋澳然 on 2021/7/28.
// email: 875691208@qq.com
// $desc
//

#ifndef EPOLLLEARN_THREAD_POOL_H
#define EPOLLLEARN_THREAD_POOL_H

#include "atomic"
#include "mutex"
#include "memory"
#include "functional"
#include "vector"
#include "unistd.h"
#include "queue"

#define COERCE_IN(a, l, r) (a) < (l) ? (l) : ((a) > (r) ? (r) : (a))

#ifdef _GNU_SOURCE

#include "sys/sysinfo.h"

const static int CPU_CORE = get_nprocs_conf();
#else

#include "thread"

const static int CPU_CORE = std::thread::hardware_concurrency();
#endif

const int THREADPOOL_INVALID = -1;
const int THREADPOOL_LOCK_FAILURE = -2;
const int THREADPOOL_QUEUE_FULL = -3;
const int THREADPOOL_SHUTDOWN = -4;
const int THREADPOOL_THREAD_FAILURE = -5;
const int THREADPOOL_GRACEFUL = -6;

const int MAX_THREADS = 1 << 10;    // 1024
const int MAX_QUEUE = (1 << 16) - 1;    // 65536

enum shutdown_mode {
    IMMEDIATE_SHUTDOWN = 1,
    GRACEFUL_SHUTDOWN = 2
};

struct thread_pool_task {
    std::function<void(std::shared_ptr<void>)> fun;
    std::shared_ptr<void> args;

    void run() const {
        fun(args);
    }
};


class thread_pool : noncopyable {
private:
    static thread_pool *m_threadpool_;
    static std::mutex mtx_;
public:

    thread_pool(const thread_pool &other) = delete;

    thread_pool &operator=(const thread_pool &other) = delete;

    int init(int _thread_count, int _queue_size = 1 << 11);   // 2048

    int submit(
            const std::shared_ptr<void> &args,
            const std::function<void(std::shared_ptr<void>)> &func
    );

    int free_resource();

    static thread_pool *instance();

    static void shutdown(shutdown_mode mode = GRACEFUL_SHUTDOWN);

    static std::string current_thread_name();

private:
    thread_pool();

    virtual ~thread_pool() {
        printf("thread_pool now has been shutdown\n");
    };

    static void *thread_routine(void *args);

    int shutdown_interval(shutdown_mode mode);

    pthread_mutex_t lock_;
    pthread_cond_t cond_;

    std::vector<pthread_t> threads_;
    std::vector<thread_pool_task> work_queue;
    std::atomic_int16_t thread_count_;
    int queue_size_;
    int curr_;
    int tail_;
    int work_count;
    std::atomic_bool shutdown_;
    volatile int shutdown_mode_;
    std::atomic_int16_t started_num_;
    // cpu集可以认为是一个掩码，每个设置的位都对应一个可以合法调度的 cpu，
    // 而未设置的位则对应一个不可调度的 CPU。换而言之，线程都被绑定了，
    // 只能在那些对应位被设置了的处理器上运行。
    // 通常，掩码中的所有位都被置位了，也就是可以在所有的cpu中调度。
    cpu_set_t mask{};
};

#endif //EPOLLLEARN_THREAD_POOL_H
