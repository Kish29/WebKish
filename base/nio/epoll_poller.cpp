//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#include "epoll_poller.h"
#include "logger.h"

// EPOLL_CLOEXEC
// 在进程执行exec系统调用时关闭此打开的文件描述符。防止父进程泄露打开的文件给子进程，即便子进程没有相应权限。
// fd泄露引起普通用户访问无权限的文件
kish::epoll_poller::epoll_poller() : epoll_fd(epoll_create1(EPOLL_CLOEXEC)), query_list(KINIT_EVENT_SIZE) {}

void epoll_poller::addev(const handler_ptr &obs) {
    if (obs) {
        save_map.insert(std::make_pair(obs->fd(), obs));
        struct epoll_event ev{};
        ev.data.fd = obs->fd();
        ev.events = obs->events();
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, obs->fd(), &ev) == -1) {
            LOG_TRACE << "epoll_poller[" << epoll_fd << "] epoll_add fd: " << obs->fd() << " failed";
        } else {
            LOG_TRACE << "epoll_poller[" << epoll_fd << "] epoll_add fd: " << obs->fd() << " success";
        }
    }
}

handler_list &epoll_poller::poll(int timeout) {
    // 检查dead对象，并移除
    update_savemap();
#ifdef __DEBUG__
    // 注意，maxevents不能为0!!!
    {
        LOG_INFO << "epoll_poller[" << epoll_fd << "] to poll-> timeout: " << timeout << ", map size: " << save_map.size();
    }
#endif

    int readyn = ::epoll_wait(epoll_fd, &(*query_list.begin()), (int) query_list.size(), timeout);

#ifdef __DEBUG__
    {
        if (readyn > 0) {
            LOG_INFO << "epoll_poller[" << epoll_fd << "] readyn is: " << readyn;
        }
    }
#endif

    ret_list.clear();
    if (readyn > 0) {
        ret_list.reserve(readyn);
        for (int i = 0; i < readyn; ++i) {
            int curr_fd = query_list.at(i).data.fd;
#ifdef __DEBUG__
            {
                LOG_INFO << "epoll_poller[" << epoll_fd << "] handle fd[" << curr_fd << "]";
            }
#endif
            uint32_t curr_ev = query_list.at(i).events;
            // 获取保存的epoll_handler指针
            if (save_map.find(curr_fd) != save_map.end()) {
                handler_weak_ptr h = save_map.at(curr_fd);
                if (h.lock()) {
                    h.lock()->update_latest_events(curr_ev);
                    ret_list.emplace_back(h);
                }
            }
        }
    } else if (readyn < 0) {
        LOG_FATAL << "epoll_poller[" << epoll_fd << "] epoll_wait failed!";
    }
    return ret_list;
}

void epoll_poller::update_savemap() {
    auto it = save_map.begin();
    while (it != save_map.end()) {
        if (it->second.lock() == nullptr || it->second.lock()->dead()) {
            struct epoll_event ev{};
            ev.data.fd = it->first;     // fd
            ev.events = KNONE_EVENT;
            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, it->first, &ev);
            save_map.at(it->first).reset();
            it = save_map.erase(it);
        } else {
            it++;
        }
    }
}

void epoll_poller::delev(const handler_ptr &obs) {
    if (obs && save_map.find(obs->fd()) != save_map.end()) {
        struct epoll_event ev{};
        ev.data.fd = obs->fd();
        ev.events = obs->events();
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, obs->fd(), &ev);
        save_map.erase(obs->fd());
    }
}

void epoll_poller::modev(const handler_ptr &obs) {
    if (obs && save_map.find(obs->fd()) != save_map.end()) {
        // epoll_add比epoll_mod更快！
        struct epoll_event ev{};
        ev.data.fd = obs->fd();
        ev.events = obs->events();
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, obs->fd(), &ev);
    }
}

