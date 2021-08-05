//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_BASE_H
#define WEBKISH_BASE_H

#include "functional"
#include "pthread.h"
#include "memory"
#include "unistd.h"
#include "cassert"
#include "cstdio"
#include "string"
#include "atomic"

// 86400 = 60 * 60 * 24
#define SLEEP_ADAY sleep(86400)

namespace kish {

    using std::function;
    using std::shared_ptr;

    typedef function<void *(void *)> callback;
    typedef function<void()> callable;

    class noncopyable {
    public:
        noncopyable(const noncopyable &) = delete;

        noncopyable &operator=(const noncopyable &) = delete;

    protected:
        noncopyable() = default;

        virtual ~noncopyable() = default;
    };

    class copyable {
    protected:
        copyable() = default;

        virtual ~copyable() = default;

    };

    class printable {
    public:
        virtual std::string tostring() const = 0;
    };

    class jsonable {
    public:
        virtual std::string tojson() const = 0;
    };

    // 返回报文结构
    class message_type {
    public:
        virtual std::string tomessage() = 0;
    };

    template<class T>
    class singleton {
    public:

        singleton() = delete;

        ~singleton() = delete;

        singleton(const singleton<T> &) = delete;

        singleton(singleton<T> &&) = delete;

        singleton &operator=(const singleton<T> &) = delete;

        singleton &operator=(singleton<T> &&) = delete;

        static T &instance() {
            // 由于指令的乱序执行，double check locking靠不住
            pthread_once(&p_once, init);
            assert(inst != nullptr);
            return *inst;
        }

    private:
        static void init() {
            inst = new T;
            ::atexit(when_exit);
        }

        static void when_exit() {
            delete inst;
            inst = nullptr;
        }

    private:
        static pthread_once_t p_once;
        static T *inst;

    };

    template<class T>
    pthread_once_t singleton<T>::p_once = PTHREAD_ONCE_INIT;
    template<class T>
    T *singleton<T>::inst = nullptr;

    // 因为log日志中会调用tid，为减少频繁的系统调用造成的上下文切换开销
    // tid会取保存的tid
    extern pid_t tid();

    extern const char *curr_thread_name();
}

#endif //WEBKISH_BASE_H
