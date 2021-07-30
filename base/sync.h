//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_SYNC_H
#define WEBKISH_SYNC_H

#include "ctime"
#include "base.h"
#include "cerrno"
#include "cstdint"
#include "pthread.h"

// 核心类，同步用
namespace kish {
    class mutex_lock;

    class mutex_cond;

    class mutex_lockguard;

    class mutex_lock : noncopyable {

    public:
        mutex_lock() {
            pthread_mutex_init(&mutex_, nullptr);
        }

        ~mutex_lock() {
            pthread_mutex_lock(&mutex_);
            pthread_mutex_destroy(&mutex_);
        }

        void lock() {
            pthread_mutex_lock(&mutex_);
        }

        void unlock() {
            pthread_mutex_unlock(&mutex_);
        }

    private:
        pthread_mutex_t mutex_{};

    private:
        friend class mutex_cond;
    };

    class mutex_lockguard : noncopyable {
    public:
        explicit mutex_lockguard(mutex_lock &_locker) : locker_(_locker) {
            locker_.lock();
        }

        ~mutex_lockguard() {
            locker_.unlock();
        }

        void unlock() const {
            locker_.unlock();
        }

    private:
        mutex_lock &locker_;
    };

    class mutex_cond : noncopyable {
    public:

        explicit mutex_cond(mutex_lock &_locker) : locker_(_locker) {
            pthread_cond_init(&cond_, nullptr);
        }

        void notify_one() {
            pthread_cond_signal(&cond_);
        }

        void notify_all() {
            pthread_cond_broadcast(&cond_);
        }

        void wait() {
            pthread_cond_wait(&cond_, &(locker_.mutex_));
        }

        bool wait_for(uint32_t seconds) {
            timespec time{};
            clock_gettime(CLOCK_REALTIME, &time);
            // 设定超时时间点
            time.tv_sec += static_cast<time_t>(seconds);
            // 返回值为true表示超时了获取了锁
            return ETIMEDOUT == pthread_cond_timedwait(&cond_, &(locker_.mutex_), &time);
        }

    private:
        mutex_lock &locker_;
        pthread_cond_t cond_{};

    };

}
#endif //WEBKISH_SYNC_H
