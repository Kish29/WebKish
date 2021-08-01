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

    // 核心类，pthread的封装
    // todo: 获取CPU_SET集，设置CPU亲和性
    class thread {
    public:

        thread(thread_func func, std::string n, bool setaffinity = true);

        ~thread();

        virtual void start();

        bool judge_in_thread() const;

        virtual int join() final;

        virtual int detach() final;

        const std::string &t_name() const;

    protected:
        thread_func exe;
        std::string name;
        // todo: 注意，pid_t和thread_t是完全不同的两个东西
        // pid_t 用于gettid判断，pthread_t表示某个posix线程
        pid_t t_tid{};  // atomic
        pthread_t pt_id{};
        // todo: volatile is safe?
        volatile bool started{false};
        volatile bool finished{false};
        volatile bool joined{false};

    private:
        friend void *thread_exe(void *);
    };
}


#endif //WEBKISH_THREAD_H
