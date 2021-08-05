//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "looper_pool.h"

#include "logger.h"
#include "util.h"

looper_pool::looper_pool(int looper_num) {
    looper_num = coerce_in(looper_num, CPU_CORE, KMAX_THREADS);
    LOG_TRACE << "looper-pool init " << looper_num << " loopers";
    loopers.reserve(looper_num);
    for (int i = 0; i < looper_num; ++i) {
        loopers.emplace_back(looper_info{
                0,
                std::make_shared<event_looper>("looper-pool-looper" + std::to_string(i))});
    }
}

looper_ptr looper_pool::next_loop() {
    // robin算法
    looper_ptr l = loopers.at(next).looper;
    next++;
    next %= loopers.size();
    return l;
}

void looper_pool::start_loop_pool() const {
    for (const looper_info &lp: loopers) {
        if (lp.looper) {
            lp.looper->start();
        } else {
            LOG_FATAL << "looper pool has looper of nullptr!";
            ::abort();
        }
    }
    // 等待所有的looper都已经开始了
    bool all_started;
    do {
        all_started = true;
        for (const looper_info &lp: loopers) {
            all_started &= lp.looper->loop_started;
        }
    } while (!all_started);
#ifdef __DEBUG__
    LOG_INFO << "looper pool has started all loopers!";
#endif
}
