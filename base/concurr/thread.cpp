//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#include "thread.h"
#include "cassert"
#include "logger.h"

using namespace kish;

__thread thread_cache *t_cache_data = nullptr;
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
            LOG_TRACE << "thread_exe -> instance is null";
            pthread_exit(nullptr);
        }
        t_cache_data = new thread_cache;
        t_cache_data->tid = kish::tid();
        instance->m_tid = t_cache_data->tid;
        t_cache_data->thread_name = instance->m_name;
        LOG_TRACE << "new thread [" << t_cache_data->thread_name << "] created and started";
        try {
            instance->m_exe();
            LOG_TRACE << "thread [" << t_cache_data->thread_name << "] exit";
        } catch (const std::exception &exc) {
            // todo: log error
        } catch (...) {
            // todo: log error
            throw;
        }
        // todo: check
        delete t_cache_data;
        pthread_exit(nullptr);
    }

    kish::thread::thread(thread_func exe, std::string name, bool setaffinity) : m_exe(std::move(exe)),
                                                                                m_name(std::move(name)) {}

    bool kish::thread::judge_in_thread() const {
        if (t_cache_data == nullptr) return false;
        return m_tid == t_cache_data->tid;
    }

    void kish::thread::start() {
        if (pthread_create(&m_ptid, nullptr, thread_exe, this)) {
            m_started = true;
        } else {
            // todo: log error
            m_started = false;
        }
    }

    int thread::join() {
        // assert(!m_joined);   // 这儿断言存在问题，join有可能在其他地方被调用几次
        if (!m_joined) {
            m_joined = true;
            return pthread_join(m_ptid, nullptr);
        }
        return 0;
    }

    const std::string &thread::name() const {
        return m_name;
    }

    thread::~thread() {
        m_finished = true;
        if (m_started && !m_joined) {
            detach();
        }
    }

    int thread::detach() {
        return pthread_detach(m_ptid);
    }

}
