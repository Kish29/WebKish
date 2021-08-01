//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_POLL_POLLER_H
#define WEBKISH_POLL_POLLER_H

#include "poller.h"
#include "sys/poll.h"

namespace kish {

    // todo: poll sys call implementation
    class poll_poller : public poller {
    public:
        handler_list &poll(int timeout) override;

        void addev(const handler_ptr &obs) override;

        void delev(const handler_ptr &obs) override;

        void modev(const handler_ptr &obs) override;

    };

}


#endif //WEBKISH_POLL_POLLER_H
