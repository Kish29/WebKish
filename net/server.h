//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_SERVER_H
#define WEBKISH_SERVER_H

#include "base.h"
#include "accept_handler.h"

namespace kish {

    class server : noncopyable {
    public:
        virtual void startup() = 0;

        virtual void stop() = 0;

        virtual void on_acceptnew(int fd, const inet_address &peer_addr) = 0;
    };

}

#endif //WEBKISH_SERVER_H
