//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_THREAD_H
#define WEBKISH_THREAD_H

#include "sync.h"
#include "string"

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

    protected:
        thread_func m_exe;
        std::string m_name;
        // todo: 注意，pid_t和thread_t是完全不同的两个东西
        // pid_t 用于gettid判断，pthread_t表示某个posix线程
        pid_t m_tid{};  // atomic
        pthread_t m_ptid{};
        // todo: volatile is safe?
        volatile bool m_started{false};
        volatile bool m_finished{false};
        volatile bool m_joined{false};

    private:
        friend void *thread_exe(void *);
    };
}


#endif //WEBKISH_THREAD_H
