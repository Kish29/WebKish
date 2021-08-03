//
// Created by 蒋澳然 on 2021/8/3.
// email: 875691208@qq.com
// $desc
//

#include "thread_pool.h"

int main() {

    EXECUTOR_POOL.init(10, 1000);

    std::atomic_int count{0};
    int full_time = 0;
    for (int i = 0; i < 10000; ++i) {
        if (kish::QUEUE_FULL == EXECUTOR_POOL.submit([&]() -> void {
            count++;
        })) {
            full_time++;
        }
    }

    sleep(1);
    printf("count is %d\n full time is %d\n", static_cast<int >(count), full_time);

}
