//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "http_handler.h"
#include "cstring"
#include "socket.h"

std::vector<shared_ptr<kish::http_interface>> kish::http_handler::GLO_RESOLR_MAPPERS;

void http_handler::handle_read() {
    base::handle_read();
    if (!m_dead) {
        // build-content
        const char *body = "<h>this is what i send to you</h>";
        size_t bodylen = strlen(body);

        // build-header
        bzero(m_send_buf, KSEND_BUFSIZ);
        sprintf(m_send_buf,
                "HTTP/1.1 400 Bad Request\r\nContent-Length: %lu\r\nContent-Type: text/plain\r\n\r\n%s",
                bodylen,
                body);
//        send(m_observe_fd, m_send_buf, strlen(m_send_buf), MSG_NOSIGNAL);
        ssize_t sent = send(m_observe_fd, m_send_buf, strlen(m_send_buf), 0);
#ifdef __DEBUG__
        if (sent == -1) {
            perror("socket send error!");
        } else {
            printf("after parse header, socket send %ld bytes\n", sent);
        }
#endif

        // todo: 应当根据http版本号选择是否关闭连接
        //  ⚠️重要，必须实现！！！⚠️
        /* 按照HTTP/1,0版本，一次http请求就代表一次tcp三次握手和一次tcp四次挥手
         * HTTP/1.1版本默认保持长连接，即只有双方其中一方发起tcp挥手时，断开连接
         * 否则会出现客户端保持连接，服务端关闭，客户端再次请求出现response的pending状态
         * 或者是客户端一次性请求，服务端为关闭，客户端一直处于FIN_WAIT状态,等待服务端发送ACK
         * */
        // todo: 列子
        const char *cnn_type = "Keep-Alive";
        const char *cnn_ver = "http/1.1";
        // 1. connection = "close"
        // 2. connection != "Keep-alive" && version == "http/1.0"
        /*if (strcmp("Keep-Alive", cnn_type) != 0 && strcmp("HTTP/1.0", cnn_ver) == 0) {
            // shutdown()函数可以选择关闭全双工连接的读通道或者写通道，如果两个通道同时关闭，则这个连接不能再继续通信。
            // close()函数会同时关闭全双工连接的读写通道，除了关闭连接外，
            // !!!!还会释放套接字占用的文件描述符。!!!!
            // 而shutdown()只会关闭连接，但是不会释放占用的文件描述符。
            // 所以即使使用了SHUT_RDWR类型调用shutdown()关闭连接，也仍然要调用close()来释放连接占用的文件描述符。
            // shutdownwrite后，浏览器重复请求并刷新数据会出现pending的状态，即服务端无法再发送数据
            // 不要关闭read，让客户端发送四次挥手的第二次ACK，虽然没啥用就是了 doge
            socket_utils::shutdown_wr(m_observe_fd);
            m_dead = true;
        } else {
            m_keep_alive = true;
        }*/
    }
}

bool http_handler::keep_alive() const {
    return m_keep_alive;
}

void http_handler::set_dead() {
    m_dead = true;
}

