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
#include "time_stamp.h"


// 核心类，同步用
namespace kish {

    class mutex_lock;

    class mutex_cond;

    class mutex_lockguard;

    class mutex_lock : noncopyable {

    public:
        mutex_lock() {
            pthread_mutex_init(&mutex, nullptr);
        }

        ~mutex_lock() {
            pthread_mutex_lock(&mutex);
            pthread_mutex_destroy(&mutex);
        }

        void lock() {
            pthread_mutex_lock(&mutex);
        }

        void unlock() {
            pthread_mutex_unlock(&mutex);
        }

    private:
        pthread_mutex_t mutex{};

    private:
        friend class mutex_cond;
    };

    class mutex_lockguard : noncopyable {
    public:
        explicit mutex_lockguard(mutex_lock &_locker) : locker(_locker) {
            locker.lock();
        }

        ~mutex_lockguard() override {
            unlock_internal();
        }

        void unlock() {
            unlock_internal();
        }

    private:
        mutex_lock &locker;
        std::atomic_bool unlocked{false};

    private:
        void unlock_internal() {
            // 主要是防止外部调用者声明了生命周期（花括号包裹）后，又重复调用unlock函数
            if (!unlocked) {
                unlocked = true;
                locker.unlock();
            }
        }
    };

    class mutex_cond : noncopyable {
    public:

        explicit mutex_cond(mutex_lock &_locker) : locker(_locker) {
            pthread_cond_init(&cond, nullptr);
        }

        void notify_one() {
            pthread_cond_signal(&cond);
        }

        void notify_all() {
            pthread_cond_broadcast(&cond);
        }

        void wait() {
            pthread_cond_wait(&cond, &(locker.mutex));
        }

        bool wait_for_sec(uint32_t seconds) {
            timespec time{};
            clock_gettime(CLOCK_REALTIME, &time);
            // 设定超时时间点
            time.tv_sec += static_cast<time_t>(seconds);
            // 返回值为true表示超时了获取了锁
            return ETIMEDOUT == pthread_cond_timedwait(&cond, &(locker.mutex), &time);
        }

        bool wait_for_ms(uint32_t ms) {
            timespec time{};
            clock_gettime(CLOCK_REALTIME, &time);
            // 设定超时时间点
            // 注意计算方法
            int64_t sec_add = ms / KMILS_PER_SEC;
            int64_t nsec_add = (ms % KMILS_PER_SEC) * (KNANO_PER_SEC / KMILS_PER_SEC);
            time.tv_sec += sec_add;
            time.tv_nsec += nsec_add;
            // 返回值为true表示超时了获取了锁
            return ETIMEDOUT == pthread_cond_timedwait(&cond, &(locker.mutex), &time);
        }

    private:
        mutex_lock &locker;
        pthread_cond_t cond{};

    };

}
#endif //WEBKISH_SYNC_H
