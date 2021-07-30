//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "http_handler.h"
#include "cstring"
#include "socket.h"

void http_handler::handle_read() {
    base::handle_read();
    if (!i_am_dead) {
        // build-body
        const char *body = "<h>this is what i send to you</h>";
        size_t bodylen = strlen(body);

        // build-header
        bzero(send_buf, KSEND_BUFSIZ);
        sprintf(send_buf,
                "HTTP/1.1 400 Bad Request\r\nContent-Length: %lu\r\nContent-Type: text/plain\r\n\r\n%s",
                bodylen,
                body);
        // todo: 弄懂shutdown和close的区别
        // shutdown()函数可以选择关闭全双工连接的读通道或者写通道，如果两个通道同时关闭，则这个连接不能再继续通信。
        // close()函数会同时关闭全双工连接的读写通道，除了关闭连接外，
        // !!!!还会释放套接字占用的文件描述符。!!!!
        // 而shutdown()只会关闭连接，但是不会释放占用的文件描述符。
        // 所以即使使用了SHUT_RDWR类型调用shutdown()关闭连接，也仍然要调用close()来释放连接占用的文件描述符。
        send(observe_fd, send_buf, strlen(send_buf), MSG_NOSIGNAL);

        // todo: 应当根据http版本号选择是否关闭连接
        // todo: 列子
        const char *cnn_type = "Keep-Alive";
        const char *cnn_ver = "http/1.1";
        if (strcmp("Keep-Alive", cnn_type) != 0 || strcmp("HTTP/1.0", cnn_ver) == 0) {
            // shutdownwrite后，浏览器重复请求并刷新数据会出现pending的状态
            socket_utils::shutdown_write(observe_fd);
            i_am_dead = true;
        }
    }
}
