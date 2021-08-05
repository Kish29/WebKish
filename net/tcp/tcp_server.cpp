//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "tcp_server.h"
#include "tcp_handler.h"
#include "logger.h"

using namespace std::placeholders;
// placeholer的工作原理相当于此
// 只不过编译器会在每个具体调用的地方放上实际的参数
// int a, b;
// acceptor.on_acceptnew(std::bind(&tcp_server::set_on_acceptnew, this, a, *((kish::inet_address *) &b)));

kish::tcp_server::tcp_server(uint16_t port) : tcp_server(port, "0.0.0.0") {}

kish::tcp_server::tcp_server(uint16_t port, const string &host, int looper_num)
        : serv_sock(true, false),
          serv_addr(port, host),
          acceptor(std::make_shared<accept_handler>(serv_sock.fd(), serv_sock)),
          loopers(looper_num) {
    serv_sock.reuse_addr(true);
    if (!serv_sock.workon(serv_addr)) {
        LOG_FATAL << "server work on " << serv_addr.ip_port() << " failed!";
        abort();
    } else {
        LOG_TRACE << "Server stated at " << serv_addr.ip_port() << " success!";
    }
    acceptor->set_on_acceptnew(std::bind(&tcp_server::on_acceptnew, this, std::placeholders::_1, std::placeholders::_2));
}

void tcp_server::on_acceptnew(int fd, const inet_address &peer_addr) {
    LOG_TRACE << "new connection from " << peer_addr.ip_port() << " for new connector[" << fd << "]";
    looper_ptr looper = loopers.next_loop();
    // ❌️这样写是不会有计数的！！！！
    // ❌️智能指针教科书式的错误用法
    // shared_ptr<epoll_handler> eh(dynamic_cast<epoll_handler *>(acceptor.get()));
    // shared_ptr<epoll_handler> eh(dynamic_cast<epoll_handler *>(tcp_obs.get()));
    looper->add_observe(handler_ptr(new tcp_handler(fd)));;

}

void tcp_server::startup() {
    /* ❌禁止进行栈空间构造event_looper ❌️*/
    // event_looper looper;
    loopers.start_loop_pool();
    LOG_TRACE << "looper pool started";
    looper_ptr looper = loopers.next_loop();
    if (acceptor) {
        looper->add_observe(acceptor);
    } else {
        LOG_FATAL << "looper[" << looper->thread_pid() << "] acceptor is nullptr!";
    }
}

void tcp_server::stop() {

}

