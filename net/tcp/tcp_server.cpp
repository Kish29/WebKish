//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "tcp_server.h"
#include "tcp_handler.h"

using namespace std::placeholders;
// placeholer的工作原理相当于此
// 只不过编译器会在每个具体调用的地方放上实际的参数
// int a, b;
// m_acceptor.on_acceptnew(std::bind(&tcp_server::on_acceptnew, this, a, *((kish::inet_address *) &b)));

kish::tcp_server::tcp_server(uint16_t port) : tcp_server(port, "0.0.0.0") {}

kish::tcp_server::tcp_server(uint16_t port, const string &host)
        : m_serv_sock(true, false),
          m_serv_addr(port, host),
          m_acceptor(std::make_shared<accept_handler>(m_serv_sock.fd(), m_serv_sock)) {
    m_serv_sock.reuse_addr(true);
    if (!m_serv_sock.workon(m_serv_addr)) {
        // todo: log error
        // todo: delete this print
        printf("server work on %s failed!\n", m_serv_addr.ip_port().c_str());
        abort();
    } else {
        printf("server start at %s\n", m_serv_addr.ip_port().c_str());
    }
    m_acceptor->on_acceptnew(std::bind(&tcp_server::on_acceptnew, this, std::placeholders::_1, std::placeholders::_2));
}

void tcp_server::on_acceptnew(int fd, const inet_address &peer_addr) {
    // todo: delete this print
    printf("new connection from %s\n", peer_addr.ip_port().c_str());
    m_looper.submit([&]() -> void {
        // ❌️这样写是不会有计数的！！！！
        // ❌️智能指针教科书式的错误用法
        // shared_ptr<epoll_handler> eh(dynamic_cast<epoll_handler *>(m_acceptor.get()));
        // shared_ptr<epoll_handler> eh(dynamic_cast<epoll_handler *>(tcp_obs.get()));
        std::shared_ptr<epoll_handler> tcp_obs(new tcp_handler(fd));
        if (tcp_obs) {
            m_looper.add_observe(tcp_obs);
        }
    });
}

void tcp_server::startup() {
    /* ❌禁止进行局部构造event_looper ❌️*/
    // event_looper m_looper;
    // todo: delete this printf
    printf("start m_looper\n");
    m_looper.start();
//    std::this_thread::sleep_for(std::chrono::seconds(1));
    m_looper.submit([&]() -> void {
        // ❌这样写是不会有计数的！！！！
        // ❌️智能指针教科书式的错误用法
        // shared_ptr<epoll_handler> eh(dynamic_cast<epoll_handler *>(m_acceptor.get()));
        if (m_acceptor) {
            m_looper.add_observe(m_acceptor);
        } else {
            // todo: delete this printf
            printf("dynamic_cast failed!\n");
        }
    });
}

void tcp_server::stop() {

}
