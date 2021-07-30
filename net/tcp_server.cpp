//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "tcp_server.h"

using namespace std::placeholders;
// placeholer的工作原理相当于此
// 只不过编译器会在每个具体调用的地方放上实际的参数
// int a, b;
// accep.on_acceptnew(std::bind(&tcp_server::on_acceptnew, this, a, *((kish::inet_address *) &b)));

kish::tcp_server::tcp_server(uint16_t port) : tcp_server(port, "0.0.0.0") {}

kish::tcp_server::tcp_server(uint16_t port, const string &host)
        : server_sock(true, false),
          server_addr(port, host),
          accep(std::make_shared<acceptor>(server_sock.fd(), server_sock)) {
    server_sock.reuse_addr(true);
    if (!server_sock.workon(server_addr)) {
        // todo: log error
        // todo: delete this print
        printf("server work on %s failed!\n", server_addr.ip_port().c_str());
        abort();
    } else {
        printf("server start at %s\n", server_addr.ip_port().c_str());
    }
    accep->on_acceptnew(std::bind(&tcp_server::on_acceptnew, this, std::placeholders::_1, std::placeholders::_2));
}

#include "cstring"

void tcp_server::on_acceptnew(int fd, const inet_address &peer_addr) {
    printf("new connection from %s\n", peer_addr.ip_port().c_str());
    const char *body = "<h>this is what i send to you</h>";
    size_t bodylen = strlen(body);
    char header[1024];
    sprintf(header, "HTTP/1.1 200 OK\r\nContent-length: %lu\r\n\r\n%s", bodylen, body);
    send(fd, header, strlen(header), MSG_NOSIGNAL);
    // todo: 弄懂shutdown和close的区别
    // todo：模糊：shutdownwrite后，浏览器保持连接，可以刷新，close不可以
    socket_utils::shutdown_write(fd);
    /*looper.submit([=]() -> void {
        // todo: 问题出在这里，tcp连接后仍然会发送消息，一定要读完数据
        shared_ptr<epoll_handler> eh = std::make_shared<epoll_handler>(fd);
        if (eh) {
            looper.add_observe(eh);
        }
    });*/
}

#include "thread"

void tcp_server::startup() {
    /* ⚠️警告！！！️禁止进行局部构造event_looper ⚠️*/
    // event_looper looper;
    // todo: delete this printf
    printf("start looper\n");
    looper.start();
//    std::this_thread::sleep_for(std::chrono::seconds(1));
    looper.submit([&]() -> void {
        shared_ptr<epoll_handler> eh(dynamic_cast<epoll_handler *>(accep.get()));
        if (eh) {
            looper.add_observe(eh);
        } else {
            // todo: delete this printf
            printf("dynamic_cast failed!\n");
        }
    });
}

void tcp_server::stop() {

}
