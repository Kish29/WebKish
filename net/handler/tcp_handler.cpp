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
    return KREAD_EVENT | KERROR_EVENT ;
}

void tcp_handler::handle_read() {
    // tcp处理层必须提供正确的事件判断，才能传给上一层协议进行判断
    ssize_t n = readin();
    if (n > 0) {
        // todo: delete this print
        // todo: log not print
        printf("observable send new message:\n%s\n", read_buf);
    } else if (n == 0) {
        perror("tcp handler read fd 0");
        // ❌不准单纯用 n == 0来表示客户端断开了连接❌
        // 方法一
        /*int save_errno = errno;
        // EINTR表示是由系统打断的通知，不应该认为客户端断开了连接
        if (save_errno != EINTR) {
            i_am_dead = true;
        }*/
        // 方法二
        i_am_dead = socket_utils::tcp_isdead(observe_fd);
    } else {
        perror("tcp handler read fd -1");
        // ❌也不准单纯用 n == -1来表示客户端断开了连接❌
        int save_errno = errno;
        switch (errno) {
            case EAGAIN: {
                // EAGAGIN表示observe_fd在非阻塞模式下执行了阻塞操作
                // 比如nc 命令等待输入，这时候也不认为断开了连接
            }
            case EINTR: {
                // EINTR表示是由系统打断的通知，不应该认为客户端断开了连接
                return;
            }
                // todo: 待完善判断
            case EIO:
            default:
                i_am_dead = true;
        }
    }
}
