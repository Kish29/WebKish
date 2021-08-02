//
// Created by 蒋澳然 on 2021/7/28.
// email: 875691208@qq.com
// $desc
//

#include "thread_pool.h"
#include "cassert"

std::mutex thread_pool::mtx_{};
thread_pool *thread_pool::m_threadpool_ = nullptr;

/*pthread_mutex_t thread_pool::lock_ = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t thread_pool::cond_ = PTHREAD_COND_INITIALIZER;

std::vector<pthread_t> thread_pool::threads_{};
std::vector<thread_pool_task> thread_pool::work_queue{};
int thread_pool::thread_count_ = 0;
int thread_pool::queue_size_ = 0;
int thread_pool::curr_ = 0;
int thread_pool::tail_ = 0;
int thread_pool::work_count = 0;
int thread_pool::shutdown_mode_ = 0;
int thread_pool::started_num_ = 0;*/

thread_pool::thread_pool() :
        lock_(PTHREAD_MUTEX_INITIALIZER),
        cond_(PTHREAD_COND_INITIALIZER),
        threads_({}),
        work_queue({}),
        thread_count_(0),
        queue_size_(0),
        curr_(0),
        tail_(0),
        work_count(0),
        shutdown_(false),
        shutdown_mode_(GRACEFUL_SHUTDOWN),
        started_num_(0) {}

int thread_pool::init(int _thread_count, int _queue_size) {
    // 参数合法化
    _thread_count = COERCE_IN(_thread_count, CPU_CORE, MAX_THREADS);
    queue_size_ = COERCE_IN(_queue_size, 1024, MAX_QUEUE);

    thread_count_ = 0;
    threads_.resize(_thread_count);
    work_queue.resize(queue_size_);

    curr_ = tail_ = work_count = 0;
    started_num_ = 0;

    for (int i = 0; i < _thread_count; ++i) {
        // 设置cpu亲和性
        CPU_ZERO(&mask);
        // 将（i % CPU_CORE）号CPU加入集合中
        CPU_SET(i % CPU_CORE, &mask);
        if (pthread_create(
                &threads_.at(i),
                nullptr,
                thread_routine,
                this) != 0) {
            return -1;
        }
    }

//    args->instance = nullptr;
//    delete args;
//    args = nullptr;

    printf("thread_pool init! thread_num is %d\n", _thread_count);

    return 0;
}

thread_pool *thread_pool::instance() {
    if (m_threadpool_ == nullptr) {
        std::unique_lock<std::mutex> lck(mtx_);
        if (m_threadpool_ == nullptr) {
            m_threadpool_ = new thread_pool;
        }
        lck.unlock();
    }
    return m_threadpool_;
}

void thread_pool::shutdown(shutdown_mode mode) {
    if (m_threadpool_ != nullptr) {
        std::unique_lock<std::mutex> lck(mtx_);
        if (m_threadpool_ != nullptr) {
            m_threadpool_->shutdown_interval(mode);
            delete m_threadpool_;
            m_threadpool_ = nullptr;
        }
        lck.unlock();
    }
}

int thread_pool::submit(
        const std::shared_ptr<void> &args,
        const std::function<void(std::shared_ptr<void>)> &func
) {
    int next{}, err = 0;
    if (pthread_mutex_lock(&lock_) != 0) {
        return THREADPOOL_LOCK_FAILURE;
    }

    do {
        // 工作队列已满
        if (work_count == queue_size_) {
            err = THREADPOOL_QUEUE_FULL;
            break;
        }
        // 线程池已经关闭
        if (shutdown_) {
            err = THREADPOOL_SHUTDOWN;
            break;
        }
        next = (tail_ + 1) % queue_size_;
        work_queue.at(tail_).fun = func;
        work_queue.at(tail_).args = args;

        tail_ = next;
        work_count++;

        // 唤醒一个线程
//        printf("signal one thread\n");
        if (pthread_cond_signal(&cond_) != 0) {
            err = THREADPOOL_LOCK_FAILURE;
            break;
        }
    } while (false);

    // 释放锁
    if (pthread_mutex_unlock(&lock_) != 0) {
        err = THREADPOOL_LOCK_FAILURE;
    }
    return err;
}

int thread_pool::shutdown_interval(shutdown_mode mode) {
    int err = 0;
    if (pthread_mutex_lock(&lock_) != 0) {
        return THREADPOOL_LOCK_FAILURE;
    }

    do {
        if (shutdown_) {
            err = THREADPOOL_SHUTDOWN;
            break;
        }

        // 设置shutdown模式
        shutdown_mode_ = mode;
        shutdown_ = true;
        // 通知所有线程shutdown
        if (pthread_cond_broadcast(&cond_) != 0 ||
            pthread_mutex_unlock(&lock_) != 0) {
            err = THREADPOOL_LOCK_FAILURE;
            break;
        }

        for (int i = 0; i < thread_count_; ++i) {
            if (pthread_join(threads_.at(i), nullptr) != 0) {
                err = THREADPOOL_THREAD_FAILURE;
                break;
            }
        }

    } while (false);

    if (!err) {
        free_resource();
    }
    return 0;
}

void *thread_pool::thread_routine(void *args) {
    assert(args != nullptr);
    auto *instance = (thread_pool *) args;

    // set name
    instance->started_num_++;
    std::string name = "thread-" + std::to_string(instance->thread_count_++);
    pthread_setname_np(pthread_self(), name.c_str());
    // pthread_setaffinity_np将从CPU集合中找到一个并附着
    pthread_setaffinity_np(pthread_self(), sizeof instance->mask, &instance->mask);

    while (true) {
//        printf("thread is running...\n");
        thread_pool_task task;
        // 加锁，从工作队列中拿任务
        pthread_mutex_lock(&instance->lock_);
        while (!instance->shutdown_ && instance->work_count == 0) {
//            printf("thread wait\n");
            pthread_cond_wait(&instance->cond_, &(instance->lock_));
        }
        if (instance->shutdown_) {
            if (instance->shutdown_mode_ == IMMEDIATE_SHUTDOWN) {
                // 关闭线程池
                instance->started_num_--;
                pthread_mutex_unlock(&(instance->lock_));
                break;
            } else {
                printf("graceful shutdown threadpool\n");
                /* TODO: 优雅关闭应当等待任务队列中的任务都完成再关闭，并设置最大超时时间 */
                // 关闭线程池
                instance->started_num_--;
                pthread_mutex_unlock(&(instance->lock_));
                break;
            }
        }
        task.fun = instance->work_queue[instance->curr_].fun;
        task.args = instance->work_queue[instance->curr_].args;

        instance->work_queue[instance->curr_].fun = nullptr;
        instance->work_queue[instance->curr_].args.reset();

        instance->curr_ = (instance->curr_ + 1) % instance->queue_size_;
        instance->work_count--;
        // 释放锁
        pthread_mutex_unlock(&(instance->lock_));
        task.run();
//        sleep(1);
    }
    pthread_exit(nullptr);
}

int thread_pool::free_resource() {
    if (started_num_ > 0) {
        return -1;
    }
    pthread_mutex_lock(&lock_);
    pthread_mutex_destroy(&lock_);
    pthread_cond_destroy(&cond_);
    return 0;
}

std::string thread_pool::current_thread_name() {
#define BUFF_SIZE 255
    char buff[BUFF_SIZE];
    pthread_getname_np(pthread_self(), buff, BUFF_SIZE);
    return buff;
}
