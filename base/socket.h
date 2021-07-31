//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_SOCKET_H
#define WEBKISH_SOCKET_H

#include "sys/socket.h"
#include "fdholder.h"
#include "inet_address.h"

namespace kish {

    class socket;

    class socket_utils;

    // 核心类，对系统调用socket的部分封装
    class socket : fdholder {
    public:
        explicit socket(bool tcp = true, bool ipv6 = false);

        ~socket() override {
            if (!clsd) {
                // todo: verify what will happen when fd is -1 to be closed
                ::close(sockfd);
            }
        }

        int fd() const override {
            return sockfd;
        }

        bool bind(const inet_address &addr) const;

        bool listen(const inet_address &addr) const;

        bool workon(const inet_address &addr) const;

        void close();

        void reuse_addr(bool on) const;

        void reuse_port(bool on) const;

        void keep_alive(bool on) const;

        void tcp_nodelay(bool on) const;

        void shutdown_write() const;

    protected:
        int sockfd{-1};
        bool clsd{false};
    };

    class socket_utils {
    public:
        static int create(bool tcp = true, bool ipv6 = false);

        static bool bind(int sockfd, const inet_address &addr);

        static bool listen(int sockfd, const inet_address &addr);

        static bool work_on(int sockfd, const inet_address &addr);

        static void close(int sockfd);

        static void reuseaddr(int sockfd, bool on);

        static void reuse_port(int sockfd, bool on);

        static void keep_alive(int sockfd, bool on);

        static void tcp_nodelay(int sockfd, bool on);

        static void shutdown_read(int sockfd);

        static void shutdown_write(int sockfd);

        static void shutdown_rdwr(int sockfd);

        static bool tcp_isdead(int sockfd);
    };

}

#endif //WEBKISH_SOCKET_H
