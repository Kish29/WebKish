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
            pthread_mutex_init(&m_mutex, nullptr);
        }

        ~mutex_lock() {
            pthread_mutex_lock(&m_mutex);
            pthread_mutex_destroy(&m_mutex);
        }

        void lock() {
            pthread_mutex_lock(&m_mutex);
        }

        void unlock() {
            pthread_mutex_unlock(&m_mutex);
        }

    private:
        pthread_mutex_t m_mutex{};

    private:
        friend class mutex_cond;
    };

    class mutex_lockguard : noncopyable {
    public:
        explicit mutex_lockguard(mutex_lock &_locker) : m_locker(_locker) {
            m_locker.lock();
        }

        ~mutex_lockguard() {
            m_locker.unlock();
        }

        void unlock() const {
            m_locker.unlock();
        }

    private:
        mutex_lock &m_locker;
    };

    class mutex_cond : noncopyable {
    public:

        explicit mutex_cond(mutex_lock &_locker) : m_locker(_locker) {
            pthread_cond_init(&m_cond, nullptr);
        }

        void notify_one() {
            pthread_cond_signal(&m_cond);
        }

        void notify_all() {
            pthread_cond_broadcast(&m_cond);
        }

        void wait() {
            pthread_cond_wait(&m_cond, &(m_locker.m_mutex));
        }

        bool wait_for_sec(uint32_t seconds) {
            timespec time{};
            clock_gettime(CLOCK_REALTIME, &time);
            // 设定超时时间点
            time.tv_sec += static_cast<time_t>(seconds);
            // 返回值为true表示超时了获取了锁
            return ETIMEDOUT == pthread_cond_timedwait(&m_cond, &(m_locker.m_mutex), &time);
        }

        bool wait_for_ms(uint32_t ms) {
            timespec time{};
            clock_gettime(CLOCK_REALTIME, &time);
            // 设定超时时间点
            // 注意计算方法
            int64_t nsecs = ms * (KNANO_PER_SEC / KMILS_PER_SEC);
            time.tv_sec += nsecs / KNANO_PER_SEC;
            time.tv_nsec += (nsecs % KNANO_PER_SEC);
            // 返回值为true表示超时了获取了锁
            return ETIMEDOUT == pthread_cond_timedwait(&m_cond, &(m_locker.m_mutex), &time);
        }

    private:
        mutex_lock &m_locker;
        pthread_cond_t m_cond{};

    };

}
#endif //WEBKISH_SYNC_H
