//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_LOOPER_POOL_H
#define WEBKISH_LOOPER_POOL_H

#include "base.h"
#include "event_looper.h"

namespace kish {

    typedef std::shared_ptr<event_looper> looper_ptr;

    // 核心类，包含loopers，使用处理率算法分配looper
    class looper_pool : noncopyable {
    public:

        explicit looper_pool(int looper_num = CPU_CORE);

        void start_loop_pool() const;

        looper_ptr next_loop();

    private:
        typedef struct looper_info {
            uint64_t last_polled_events;   // 记录上一次某个looper触发的IO事件数量
            looper_ptr looper;
        } looper_info;
        std::vector<looper_info> loopers;
        size_t next{0};
    };

}


#endif //WEBKISH_LOOPER_POOL_H
