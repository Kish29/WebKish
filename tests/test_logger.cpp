//
// Created by 蒋澳然 on 2021/8/2.
// email: 875691208@qq.com
// $desc
//

#include "logger.h"

// 14 lines
void type_test() {
    LOG_INFO << "------- type convert test -------";
    LOG_INFO << true;
    LOG_INFO << false;
    LOG_INFO << (short) 100;
    LOG_INFO << (unsigned short) 200;
    LOG_INFO << 12414;
    LOG_INFO << (unsigned int) 12414;
    LOG_INFO << (long) 1001234;
    LOG_INFO << (unsigned long) 1001234;
    LOG_INFO << (long long) 10012434;
    LOG_INFO << (unsigned long long) 141431001234;
    LOG_INFO << (float) 12.12412521;
    LOG_INFO << (double) 12.12412521;
    LOG_INFO << 'c';
    LOG_INFO << std::string("safsadf");
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
    LOG_INFO << "thread-" << no << " m_started";
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

// 一共58W15行 线程日志那儿有2行 thread.cpp:38 thread.cpp:41，但日志线程已经结束了，所以无法打印
int main() {
//    type_test();
//    sleep(5);
//    stress_test1();
//    sleep(5);
//    stress_test2();
//    sleep(5);
//    SLEEP_ADAY;
//    int i = 1 / 0;
    /*char buf[50];
    int res = read(-1, buf, 50);
    int se{};
    if (res == -1) {
        se = errno;
        perror("read fd:-1");
        bzero(buf, sizeof buf);
        printf("%s\n", strerror(se));
        char *out = strerror_r(se, buf, sizeof buf);
        printf("%s\n", out);
    }*/
    LOG_TRACE << "read fd:-1 for errno test";
    sleep(1);
}