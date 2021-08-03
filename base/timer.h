//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_TIMER_H
#define WEBKISH_TIMER_H

#include "base.h"
#include "thread.h"

namespace kish {

    // 我认为负责http长连接和连接保活这种工作应当是由http层的服务实现
    // 故timer应当作为http_server的一个成员
    // 并由http_server负责清理过期连接，并通知poller移除观察的文件描述符
    // 而不是像muduo库那样交给poller去做，因为poller始终只是系统层的一个io封装
    // 什么样的工作应当对应什么样的抽象层
    class timer : noncopyable {
    public:

        explicit timer(std::string name);

        ~timer();

        void async(callable &&task, uint32_t ms_t);

        void async_loop(callable &&task, uint32_t ms_t);

        void sync(callable &&task, uint32_t ms_t);

        void sync_loop(callable &&task, uint32_t ms_t);

        void stop();

    private:
        thread_ptr m_exe;
        std::string m_name;
        mutex_lock m_locker;
        mutex_cond m_cond;
        std::atomic_bool m_stop{false};

    private:
        void schedule(callable &&task, bool async, bool loop, uint32_t ms_t);

        void abandon_prevtask();

    };

}


#endif //WEBKISH_TIMER_H
