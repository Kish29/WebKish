//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#include "thread.h"
#include "logger.h"

using namespace kish;

__thread thread_cache *t_cache_data = nullptr;
__thread pid_t t_tid = -1;

namespace kish {

    cpu_set_t thread::mask;

    pthread_once_t thread::p_once = PTHREAD_ONCE_INIT;

    // 原函数在base.h中声明
    // 因为log日志中会调用tid，为减少频繁的系统调用造成的上下文切换开销
    // tid会取保存的tid
    pid_t tid() {
        if (t_tid == -1) {
            t_tid = ::gettid();
        }
        return t_tid;
    }

    extern const char *curr_thread_name() {
        if (t_cache_data) {
            return t_cache_data->thread_name.c_str();
        } else return "thread-with-no-name";
    }

    void *thread_exe(void *args) {
        auto *instance = (thread *) args;
        if (instance == nullptr) {
            LOG_TRACE << "thread_exe -> instance is null";
            pthread_exit(nullptr);
        }
        t_cache_data = new thread_cache;
        t_cache_data->tid = kish::tid();
        instance->thr_tid = t_cache_data->tid;
        t_cache_data->thread_name = instance->thr_name;
        if (instance->set_affinity) {
            if (sched_setaffinity(t_cache_data->tid, sizeof kish::thread::mask, &kish::thread::mask) == -1) {
                LOG_WARN << "thread[" << t_cache_data->tid << "] set cpu affinity failed";
            } else {
#ifdef __DEBUG__
                LOG_INFO << "thread[" << t_cache_data->tid << "] set cpu affinity success";
#endif
            }
        }
        LOG_TRACE << "new thread [" << t_cache_data->thread_name << "] created and started";
        try {
            instance->thr_exe();
            LOG_TRACE << "thread [" << t_cache_data->thread_name << "] exit";
        } catch (const std::exception &exc) {
            LOG_FATAL << "thread-start encountered internal error: " << exc.what();
        } catch (...) {
            LOG_FATAL << "thread-start encountered unknown internal error!";
            throw;
        }
        // todo: check
        delete t_cache_data;
        pthread_exit(nullptr);
    }

    kish::thread::thread(thread_func exe, std::string name, bool setaffinity)
            : thr_exe(std::move(exe)),
              thr_name(std::move(name)),
              set_affinity(setaffinity) {
        // 初始化CPU集合
        pthread_once(&p_once, []() -> void {
            CPU_ZERO(&mask);
            for (int i = 0; i < CPU_CORE; ++i) {
                CPU_SET(i, &mask);
            }
        });
    }

    bool kish::thread::judge_in_thread() const {
        if (t_cache_data == nullptr) return false;
        return thr_tid == t_cache_data->tid;
    }

    void kish::thread::start() {
        if (pthread_create(&pthr_id, nullptr, thread_exe, this)) {
            started = true;
        } else {
            // todo: log error
            started = false;
        }
    }

    int thread::join() {
        // assert(!joined);   // 这儿断言存在问题，join有可能在其他地方被调用几次
        if (!joined) {
            joined = true;
            return pthread_join(pthr_id, nullptr);
        }
        return 0;
    }

    const std::string &thread::name() const {
        return thr_name;
    }

    thread::~thread() {
        finished = true;
        if (started && !joined) {
            detach();
        }
    }

    int thread::detach() {
        return pthread_detach(pthr_id);
    }

    pid_t thread::thread_pid() const {
        return thr_tid;
    }

}
