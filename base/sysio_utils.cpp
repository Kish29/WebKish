//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "sysio_utils.h"
#include "cstdio"

bool enable_nonblock_and_cloexec(int fd) {
    int flags = ::fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int res = ::fcntl(fd, F_SETFL, flags);
    if (res == -1) {
#ifdef __DEBUG__
        // todo: delete this print
        perror("set O_NONBLOCK failed!");
#endif
        return false;
    }
    flags = ::fcntl(fd, F_GETFL, 0);
    flags |= O_CLOEXEC;
    res = ::fcntl(fd, F_SETFL, flags);
    if (res == -1) {
#ifdef __DEBUG__
        // todo: delete this print
        printf("set O_CLOEXEC failed!");
#endif
        return false;
    }
#ifdef __DEBUG__
    // todo: delete this print
    printf("set non-block for %d success!\n", fd);
#endif
    return true;
}