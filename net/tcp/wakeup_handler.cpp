//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#include "wakeup_handler.h"
#include "logger.h"

void kish::wakeup_handler::wakeup() const {
    if (observe_fd == -1) {
        return;
    }
    // !!!one不能为0!!!，否则写入不生效，无法唤醒
    uint64_t one = 1;
    if (write(observe_fd, &one, sizeof one) == -1) {
        LOG_WARN << "wakeup write failed!";
    }
}

void kish::wakeup_handler::handle_read() {
    if (observe_fd != -1) {
        uint64_t one = 1;
        if (::read(observe_fd, &one, sizeof one) == -1) {
#ifdef __DEBUG__
            LOG_INFO << "wakeup read success!";
#endif
        }
    }
}


