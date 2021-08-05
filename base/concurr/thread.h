//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_THREAD_H
#define WEBKISH_THREAD_H

#include "sync.h"
#include "string"

#ifdef _GNU_SOURCE

#include "sys/sysinfo.h"

const static int CPU_CORE = get_nprocs_conf();

const static int KMAX_THREADS = _SC_THREAD_THREADS_MAX;
#else

#include "thread"

const static int CPU_CORE = std::thread::hardware_concurrency();

const static int KMAX_THREADS = 512;
#endif

namespace kish {

    typedef std::function<void()> thread_func;

    struct thread_cache {
        pid_t tid;
        std::string thread_name;
    };

    class thread;

    typedef std::shared_ptr<thread> thread_ptr;

    // 核心类，pthread的封装
    // todo: 获取CPU_SET集，设置CPU亲和性
    class thread {
    public:

        thread(thread_func exe, std::string name, bool setaffinity = true);

        ~thread();

        virtual void start();

        bool judge_in_thread() const;

        virtual int join() final;

        virtual int detach() final;

        const std::string &name() const;

        virtual pid_t thread_pid() const final;

    protected:
        thread_func thr_exe;
        std::string thr_name;
        // pid_t 用于gettid判断，pthread_t表示某个posix线程
        pid_t thr_tid{};  // atomic
        pthread_t pthr_id{};
        bool set_affinity{true};
        // volatile 从某种层面上来说，仅仅是读安全
//        volatile bool started{false};
        std::atomic_bool started{false};
        std::atomic_bool finished{false};
        std::atomic_bool joined{false};

        static cpu_set_t mask;
        static pthread_once_t p_once;

    private:
        friend void *thread_exe(void *);
    };
}


#endif //WEBKISH_THREAD_H
