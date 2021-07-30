//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#include "epoller.h"
#include "epoll_handler.h"

// EPOLL_CLOEXEC
// 在进程执行exec系统调用时关闭此打开的文件描述符。防止父进程泄露打开的文件给子进程，即便子进程没有相应权限。
// fd泄露引起普通用户访问无权限的文件
kish::epoller::epoller() : epoll_fd(epoll_create1(EPOLL_CLOEXEC)), epoll_evlst(KINIT_EVENT_SIZE) {}

void epoller::epoll_add(const shared_ptr<epoll_handler> &h) {
    save_map.insert(std::make_pair(h->fd(), h));
    struct epoll_event ev{};
    ev.data.fd = h->fd();
    ev.events = h->events();
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, h->fd(), &ev) == -1) {
        // todo: log error
    }
}

#include "cstdio"

handler_list &epoller::poll(int timeout) {

#ifdef __DEBUG__
    // todo: delete this print
    printf("timeout is %d\n", timeout);
#endif

    // 检查dead对象，并移除
    update_savemap();
    // 注意，maxevents不能为0!!!
    int readyn = ::epoll_wait(epoll_fd, &(*epoll_evlst.begin()), (int) epoll_evlst.size(), timeout);

#ifdef __DEBUG__
    // todo: delete this print
    printf("readyn is %d\n", readyn);
#endif

    retlist.clear();
    if (readyn > 0) {
        // todo: delete this print
        retlist.reserve(readyn);
        for (int i = 0; i < readyn; ++i) {
            int curr_fd = epoll_evlst.at(i).data.fd;
            uint32_t curr_ev = epoll_evlst.at(i).events;
            // 获取保存的epoll_handler指针
            if (save_map.find(curr_fd) != save_map.end()) {
                shared_ptr<epoll_handler> h = save_map.at(curr_fd);
                h->update_latest_events(curr_ev);
                retlist.push_back(h);
            }
        }
    } else if (readyn < 0) {
        // todo: log epoll wait error
        // todo: delete this print
        perror("epoll_wait failed!\n");
    }
    return retlist;
}

void epoller::update_savemap() {
    auto tmp = handler_map(save_map);
    for (auto &m:tmp) {
        if (m.second->dead()) {
            // todo: 移除，注意这里边遍历边移除
            save_map.erase(m.first);
        }
    }
    tmp.clear();
    // todo: change to log
    printf("latest map size is %lu\n", save_map.size());
}

