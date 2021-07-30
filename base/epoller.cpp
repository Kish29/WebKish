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
kish::epoller::epoller() : epoll_fd(epoll_create1(EPOLL_CLOEXEC)), query_list(KINIT_EVENT_SIZE) {}

void epoller::epoll_add(const shared_ptr<epoll_handler> &obs) {
    if (obs) {
        save_map.insert(std::make_pair(obs->fd(), obs));
        struct epoll_event ev{};
        ev.data.fd = obs->fd();
        ev.events = obs->events();
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, obs->fd(), &ev) == -1) {
            // todo: log error
        }
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
    int readyn = ::epoll_wait(epoll_fd, &(*query_list.begin()), (int) query_list.size(), timeout);

#ifdef __DEBUG__
    // todo: delete this print
    printf("readyn is %d\n", readyn);
#endif

    ret_list.clear();
    if (readyn > 0) {
        // todo: delete this print
        ret_list.reserve(readyn);
        for (int i = 0; i < readyn; ++i) {
            int curr_fd = query_list.at(i).data.fd;
            uint32_t curr_ev = query_list.at(i).events;
            // 获取保存的epoll_handler指针
            if (save_map.find(curr_fd) != save_map.end()) {
                shared_ptr<epoll_handler> h = save_map.at(curr_fd);
                h->update_latest_events(curr_ev);
                ret_list.push_back(h);
            }
        }
    } else if (readyn < 0) {
        // todo: log epoll wait error
        // todo: delete this print
        perror("epoll_wait failed!\n");
    }
    return ret_list;
}

void epoller::update_savemap() {
    handler_map tmp = handler_map(save_map);
    for (auto &m:tmp) {
        if (m.second->dead()) {
            // todo: 待验证：在不移除fd的情况下，另一个looper分配到了同样的文件描述符时
            // todo：本looper是否会收到IO事件
            // todo: 根据epoll的manual page，文件描述符始终指向的是一个文件资源
            // todo：那么应当新到来的连接分配了一个用过的文件描述符，这个文件描述符会不会指向新的文件资源
            // todo: 将dead对象的fd从epoll的fd列表中移除
            /*struct epoll_event ev{};
            ev.data.fd = m.second->fd();
            ev.events = m.second->events();
            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, m.second->fd(), &ev);*/
            save_map.erase(m.first);
        }
    }
    tmp.clear();
    // todo: change to log
    printf("latest map size is %lu\n", save_map.size());
}

void epoller::epoll_del(const shared_ptr<epoll_handler> &obs) {
    if (obs && save_map.find(obs->fd()) != save_map.end()) {
        struct epoll_event ev{};
        ev.data.fd = obs->fd();
        ev.events = obs->events();
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, obs->fd(), &ev);
        save_map.erase(obs->fd());
    }
}

void epoller::epoll_mod(const shared_ptr<epoll_handler> &obs) {
    if (obs && save_map.find(obs->fd()) != save_map.end()) {
        // epoll_add比epoll_mod更快！
        struct epoll_event ev{};
        ev.data.fd = obs->fd();
        ev.events = obs->events();
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, obs->fd(), &ev);
    }
}

