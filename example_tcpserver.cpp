//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "tcp_server.h"

#include "sys/eventfd.h"

int epfd = epoll_create1(EPOLL_CLOEXEC);
int evfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);

void wakeup_test();

int main() {
    pthread_setname_np(pthread_self(), "main-thread");
    kish::tcp_server ts(5555);
    ts.startup();
    while (true);
}

void wakeup_test() {
    struct epoll_event ev{};
    ev.data.fd = evfd;
    ev.events = KREAD_EVENT;
    epoll_ctl(epfd, EPOLL_CTL_ADD, evfd, &ev);
    pthread_t t;
    pthread_create(&t, nullptr, [](void *) -> void * {
        sleep(1);
        uint64_t wkp = 1;
        printf("to wakeup him!\n");
        write(evfd, &wkp, sizeof wkp);
        return nullptr;
    }, nullptr);
    epoll_event evl[10];
    printf("epoll wait\n");
    epoll_wait(epfd, evl, 10, 10000);
    printf("wake up!\n");
}
