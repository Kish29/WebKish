//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#include "thread.h"

using namespace kish;

__thread thread_cache *cache_data = nullptr;

namespace kish {

    void *thread_exe(void *args) {
        auto *instance = (thread *) args;
        if (instance == nullptr) {
            // todo: log thread start error
            pthread_exit(nullptr);
        }
        // todo: delete this printf
        pthread_setname_np(pthread_self(), "thread-looper");
        printf("set current thread name : thread-looper\n");
        cache_data = new thread_cache;
        cache_data->tid = gettid();
        instance->t_tid = cache_data->tid;
        cache_data->thread_name = instance->name;
        // todo: delete this printf
        printf("run in thread %s\n", cache_data->thread_name.c_str());
        try {
            instance->exe();
            // todo: log thread finish
            // todo: delete this printf
            printf("thread %s exit\n", cache_data->thread_name.c_str());
        } catch (const std::exception &exc) {
            // todo: log error
        } catch (...) {
            // todo: log error
            throw;
        }
        pthread_exit(nullptr);
    }

    kish::thread::thread(kish::thread_func func, std::string n) : exe(std::move(func)), name(std::move(n)) {}

    bool kish::thread::judge_in_thread() const {
        // todo: should not new thread_cache and assignment
        if (cache_data == nullptr) {
            cache_data = new thread_cache;
            cache_data->tid = gettid();
            cache_data->thread_name = name;
        }
        return t_tid == cache_data->tid;
    }

    void kish::thread::stop() {
        // todo: check
        delete cache_data;
    }

    void kish::thread::start() {
        if (pthread_create(&pt_id, nullptr, thread_exe, this)) {
            started = true;
        } else {
            // todo: ???
        }
    }

    void thread::join() {

    }

}
