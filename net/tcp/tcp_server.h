//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_TCP_SERVER_H
#define WEBKISH_TCP_SERVER_H

#include "base.h"
#include "socket.h"
#include "server.h"
#include "looper_pool.h"
#include "tcp_handler.h"
#include "thread_pool.h"

namespace kish {


    class tcp_server : public server {
    public:

        explicit tcp_server(uint16_t port);

        tcp_server(uint16_t port, const string &host, int looper_num = CPU_CORE);

        void startup() override;

        void stop() override;

        void on_acceptnew(int, const inet_address &) override;

    protected:
        socket serv_sock;
        inet_address serv_addr;
        shared_ptr<accept_handler> acceptor;
        looper_pool loopers;
        bool started{};

    private:
        typedef shared_ptr<tcp_handler> tcp_handler_ptr;
        std::vector<tcp_handler_ptr> tcp_connectors;
        mutex_lock locker;
        thread_pool single_t_pool;  // 单个线程池，负责移除dead连接对象

    };

}


#endif //WEBKISH_TCP_SERVER_H
