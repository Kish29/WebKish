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
#include "cstdio"
#include "string"

// 86400 = 60 * 60 * 24
#define SLEEP_ADAY sleep(86400)

namespace kish {

    using std::function;
    using std::shared_ptr;

    typedef function<void(void *)> callback;
    typedef shared_ptr<callback> callback_ptr;

    class noncopyable {
    public:
        noncopyable(const noncopyable &) = delete;

        noncopyable &operator=(const noncopyable &) = delete;

    protected:
        noncopyable() = default;

        ~noncopyable() = default;
    };

    class copyable {
    protected:
        copyable() = default;

        ~copyable() = default;

    };

    class printable {
    public:
        virtual std::string tostring() const = 0;
    };

    template<class T>
    class singleton : noncopyable {
    public:

        singleton() = delete;

        ~singleton() = delete;

        static T &instance() {
            // 由于指令的乱序执行，double check locking靠不住
            pthread_once(&ponce, [&]() -> void {
                inst = new T;
                ::atexit([&]() -> void {
                    delete inst;
                    inst = nullptr;
                });
            });
            assert(inst != nullptr);
            return *inst;
        }

    private:
        static pthread_once_t ponce;
        static T *inst;
    };
}

#endif //WEBKISH_BASE_H
