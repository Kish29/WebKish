//
// Created by 蒋澳然 on 2021/8/2.
// email: 875691208@qq.com
// $desc
//

#include "logger.h"

// 14 lines
void type_test() {
    LOG_INFO << "------- psr_type convert test -------";
    LOG_RECOR << true;
    LOG_RECOR << false;
    LOG_RECOR << (short) 100;
    LOG_RECOR << (unsigned short) 200;
    LOG_RECOR << 12414;
    LOG_RECOR << (unsigned int) 12414;
    LOG_RECOR << (long) 1001234;
    LOG_RECOR << (unsigned long) 1001234;
    LOG_RECOR << (long long) 10012434;
    LOG_RECOR << (unsigned long long) 141431001234;
    LOG_RECOR << (float) 12.12412521;
    LOG_RECOR << (double) 12.12412521;
    LOG_RECOR << 'c';
    LOG_RECOR << std::string("safsadf");
}

// 10W lines
void stress_test1() {
    LOG_INFO << "------- single thread test -------";
    for (int i = 0; i < 100000; ++i) {
        LOG_RECOR << i;
    }
}

void *thread_func(void *arg) {
    int no = *((int *) arg);
    LOG_INFO << "thread-" << no << " started";
    for (int j = 0; j < 80000; ++j) {
        LOG_RECOR << "thread-" << no << " write j " << j;
    }
    pthread_exit(nullptr);
}

// 48W lines
void stress_test2() {
    LOG_INFO << "------- multi thread test -------";
    pthread_t pt_t[48];
    int *no = new int[48];
    for (int i = 0; i < 48; ++i) {
        no[i] = i + 1;
        // ⚠️最后一个参数，不能是栈空间变量，必须堆外存储
        pthread_create(&pt_t[i], nullptr, thread_func, no + i);
        pthread_join(pt_t[i], nullptr);
    }
}

// 一共394W15行 线程日志那儿有2行 thread.cpp:38 thread.cpp:41，但日志线程已经结束了，所以无法打印
int main() {
    type_test();
    sleep(5);
    stress_test1();
    sleep(5);
    stress_test2();
    sleep(5);
//    SLEEP_ADAY;
//    int i = 1 / 0;
    /*char fbuf[50];
    int res = read(-1, fbuf, 50);
    int se{};
    if (res == -1) {
        se = errno;
        perror("read fd:-1");
        bzero(fbuf, sizeof fbuf);
        printf("%s\n", strerror(se));
        char *out = strerror_r(se, fbuf, sizeof fbuf);
        printf("%s\n", out);
    }*/
//    LOG_TRACE << "read fd:-1 for errno test";
//    sleep(1);
}
