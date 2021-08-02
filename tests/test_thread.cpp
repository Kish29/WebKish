//
// Created by 蒋澳然 on 2021/8/3.
// email: 875691208@qq.com
// $desc
//

#include "thread.h"

int main() {
    kish::thread t([]() -> void {
        for (int i = 0; i < 3; ++i) {
            printf("thread print\n");
            sleep(1);
        }
    }, "main-test");
    t.start();
    t.join();
}
