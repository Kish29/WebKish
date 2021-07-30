//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "tcp_handler.h"
#include "sysio_utils.h"
#include "socket.h"

tcp_handler::tcp_handler(int fd) : epoll_handler(fd) {
    enable_nonblock_and_cloexec(fd);
}

uint32_t tcp_handler::events() const {
    return KREAD_EVENT | KERROR_EVENT;
}

void tcp_handler::handle_read() {
    // tcp处理层必须提供正确的事件判断，才能传给上一层协议进行判断
    ssize_t n = readin();
    if (n > 0) {
        // todo: delete this print
        // todo: log not print
        printf("observable send new message:\n%s\n", read_buf);
    } else if (n == 0) {
        // ❌不准单纯用 n == 0来表示客户端断开了连接❌
        // 方法一
        int save_errno = errno;
        if (save_errno != EINTR) {
            i_am_dead = true;
        }
        // 方法二
        // i_am_dead = socket_utils::tcp_disconn(observe_fd);
    } else {
        i_am_dead = true;
    }
}
