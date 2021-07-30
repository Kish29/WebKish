//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_SYSIO_UTILS_H
#define WEBKISH_SYSIO_UTILS_H

#include "fcntl.h"
#include "cstring"

bool enable_nonblock_and_cloexec(int fd);

#endif //WEBKISH_SYSIO_UTILS_H
