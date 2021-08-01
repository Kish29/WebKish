//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#include "thread.h"
#include "cassert"
#include "logger.h"

using namespace kish;

__thread thread_cache *cache_data = nullptr;
__thread pid_t t_tid = -1;

namespace kish {

    // 原函数在base.h中声明
    // 因为log日志中会调用tid，为减少频繁的系统调用造成的上下文切换开销
    // tid会取保存的tid
    pid_t tid() {
        if (t_tid == -1) {
            t_tid = ::gettid();
        }
        return t_tid;
    }

    void *thread_exe(void *args) {
        auto *instance = (thread *) args;
        if (instance == nullptr) {
            // todo: log thread start error
            pthread_exit(nullptr);
        }
        cache_data = new thread_cache;
        cache_data->tid = kish::tid();
        instance->t_tid = cache_data->tid;
        cache_data->thread_name = instance->name;
        LOG_TRACE << "new thread [" << cache_data->thread_name << "] created and started";
        try {
            instance->exe();
            LOG_TRACE << "thread [" << cache_data->thread_name << "] exit";
        } catch (const std::exception &exc) {
            // todo: log error
        } catch (...) {
            // todo: log error
            throw;
        }
        // todo: check
        delete cache_data;
        pthread_exit(nullptr);
    }

    kish::thread::thread(thread_func func, std::string n, bool setaffinity) : exe(std::move(func)), name(std::move(n)) {}

    bool kish::thread::judge_in_thread() const {
        if (cache_data == nullptr) return false;
        return t_tid == cache_data->tid;
    }

    void kish::thread::start() {
        if (pthread_create(&pt_id, nullptr, thread_exe, this)) {
            started = true;
        } else {
            // todo: log error
            started = false;
        }
    }

    int thread::join() {
        assert(!joined);
        joined = true;
        return pthread_join(pt_id, nullptr);
    }

    const std::string &thread::t_name() const {
        return name;
    }

    thread::~thread() {
        finished = true;
        if (started && !joined) {
            detach();
        }
    }

    int thread::detach() {
        return pthread_detach(pt_id);
    }

}
