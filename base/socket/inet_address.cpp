//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#include "inet_address.h"
#include "netio_utils.h"
#include "cstring"

#define SIZ 64

kish::inet_address::inet_address(uint16_t port, const string &host) : ipv6(false) {
    bzero(&addr, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = host2network16(port);
    addr.sin_addr.s_addr = inet_addr(host.c_str());
}

kish::inet_address::inet_address(uint16_t port, uint32_t host, bool from_net) : ipv6(false) {
    bzero(&addr, sizeof addr);
    addr.sin_family = AF_INET;
    if (from_net) {
        addr.sin_port = port;
        addr.sin_addr.s_addr = host;
    } else {
        addr.sin_port = host2network16(port);
        addr.sin_addr.s_addr = host2network32(host);
    }
}


kish::inet_address::inet_address(uint16_t port, bool loopback, bool v6) : ipv6(v6) {
    if (ipv6) {
        // todo
    } else {
        bzero(&addr, sizeof addr);
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = loopback ? INADDR_LOOPBACK : INADDR_ANY;
        addr.sin_port = host2network16(port);
    }
}

string kish::inet_address::ip_port() const {
    char buf[SIZ];
    if (ipv6) {
        // 构造前端的ip地址：  [ffff:9a9a:::0
        buf[0] = '[';
        ::inet_ntop(AF_INET6, &addr6.sin6_addr, buf + 1, SIZ - 1);
        uint16_t port = network2host16(addr6.sin6_port);
        size_t len = strlen(buf);
        snprintf(buf + len, SIZ - len, "]:%u", port);
    } else {
        ::inet_ntop(AF_INET, &addr.sin_addr, buf, SIZ);
        uint16_t port = network2host16(addr.sin_port);
        size_t len = strlen(buf);
        snprintf(buf + len, SIZ - len, ":%u", port);
    }
    return buf;
}

void kish::inet_address::change_host(const string &host) {
    if (ipv6) {
        // todo
//        addr6.sin6_addr
    } else {
        addr.sin_addr.s_addr = inet_addr(host.c_str());
    }
}

string kish::inet_address::ip() const {
    char buf[SIZ];
    if (ipv6) {
        ::inet_ntop(AF_INET6, &addr6.sin6_addr, buf, SIZ);
    } else {
        return ::inet_ntop(AF_INET6, &addr.sin_addr, buf, SIZ);
    }
    return buf;
}

uint16_t kish::inet_address::port() const {
    return network2host16(addr.sin_port);
}

const sockaddr *kish::inet_address::socketaddr() const {
    return (sockaddr *) (&addr);
}
