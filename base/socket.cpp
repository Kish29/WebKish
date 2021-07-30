//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#include "socket.h"
#include "netinet/tcp.h"

// todo: set functions return type is bool
kish::socket::socket(bool tcp, bool ipv6) : sockfd(socket_utils::create(tcp, ipv6)) {}

void kish::socket::reuse_addr(bool on) const {
    kish::socket_utils::reuse_addr(sockfd, on);
}

void kish::socket::reuse_port(bool on) const {
    kish::socket_utils::reuse_port(sockfd, on);
}

void kish::socket::keep_alive(bool on) const {
    kish::socket_utils::keep_alive(sockfd, on);
}

void kish::socket::tcp_nodelay(bool on) const {
    kish::socket_utils::tcp_nodelay(sockfd, on);
}

void kish::socket::shutdown_write() const {
    kish::socket_utils::shutdown_write(sockfd);
}

bool kish::socket::bind(const kish::inet_address &addr) const {
    return kish::socket_utils::bind(sockfd, addr);
}

bool kish::socket::listen(const inet_address &addr) const {
    return kish::socket_utils::listen(sockfd, addr);
}

bool kish::socket::workon(const kish::inet_address &addr) const {
    return this->bind(addr) && this->listen(addr);
}

void kish::socket::close() {
    // todo: check -1
    ::close(sockfd);
    clsd = true;
}

int kish::socket_utils::create(bool tcp, bool ipv6) {
    int domain = ipv6 ? AF_INET6 : AF_INET;
    int prt = ipv6 ? IPPROTO_IPV6 : IPPROTO_IP;
    int type = tcp ? SOCK_STREAM : SOCK_DGRAM;
    return ::socket(domain, type, prt);
}

bool kish::socket_utils::bind(int sockfd, const kish::inet_address &addr) {
    if (::bind(sockfd, addr.socketaddr(), sizeof *addr.socketaddr()) == -1) {
        // todo: log error
        perror("bind failed!");
        return false;
    }
    return true;
}

bool kish::socket_utils::listen(int sockfd, const kish::inet_address &addr) {
    if (::listen(sockfd, SOMAXCONN) == -1) {
        // todo: log error
        perror("listen failed!");
        return false;
    }
    return true;
}

bool kish::socket_utils::workon(int sockfd, const kish::inet_address &addr) {
    return socket_utils::bind(sockfd, addr) && socket_utils::listen(sockfd, addr);
}

void kish::socket_utils::close(int sockfd) {
    // todo: check -1
    ::close(sockfd);
}

void kish::socket_utils::reuse_addr(int sockfd, bool on) {
#ifdef SO_REUSEADDR
    int optval = on ? 1 : 0;
    if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) == -1) {
        // todo: log error
    }
#else
    // todo: log not support
#endif
}

void kish::socket_utils::reuse_port(int sockfd, bool on) {
#ifdef SO_REUSEADDR
    int optval = on ? 1 : 0;
    if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof optval) == -1) {
        // todo: log error
    }
#else
    // todo: log not support
#endif
}

void kish::socket_utils::keep_alive(int sockfd, bool on) {
    int optval = on ? 1 : 0;
    if (::setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof optval) == -1) {
        // todo: log error
    }
}

void kish::socket_utils::tcp_nodelay(int sockfd, bool on) {
    int optval = on ? 1 : 0;
    if (::setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof optval) == -1) {
        // todo: log error
    }
}

void kish::socket_utils::shutdown_write(int sockfd) {
    if (::shutdown(sockfd, SHUT_WR) == -1) {
        // todo: log error
    }
}
