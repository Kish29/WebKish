//
// Created by 蒋澳然 on 2021/8/3.
// email: 875691208@qq.com
// $desc
//


#include "timer.h"

int main() {
    kish::timer t("test-timer");
    t.async([]() -> void {
        printf("3s has passed\n");
        printf("hello world\n");
        fflush(stdout);
    }, 3000);
    sleep(1);
    t.stop();
    printf("exit!\n");
}