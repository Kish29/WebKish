//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_INET_ADDRESS_H
#define WEBKISH_INET_ADDRESS_H

#include "arpa/inet.h"
#include "string"
#include "base.h"

using std::string;

namespace kish {

    // 对ip和port的封装类
    class inet_address : copyable {
    public:

        inet_address(uint16_t port, const string &host);

        // from_net表示数据是否从网络中接受而来
        inet_address(uint16_t port, uint32_t host, bool from_net);

        // 构造函数默认生成"0.0.0.0/::0"的地址，如果设置了loopback，则为127.0.0.1
        explicit inet_address(uint16_t port, bool loopback = false, bool v6 = false);

        void change_host(const string &host);

        string ip_port() const;

        string ip() const;

        uint16_t port() const;

        const sockaddr *socketaddr() const;

    private:
        bool m_ipv6;
        union {
            sockaddr_in m_addr;
            sockaddr_in6 m_addr6;
        };
    };
}


#endif //WEBKISH_INET_ADDRESS_H
