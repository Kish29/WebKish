//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_SELECT_POLLER_H
#define WEBKISH_SELECT_POLLER_H

#include "poller.h"
#include "sys/select.h"

namespace kish {

    // todo: select sys call implementation
    class select_poller : public poller {
    public:

        handler_list &poll(int timeout) override;

        void addev(const handler_ptr &obs) override;

        void delev(const handler_ptr &obs) override;

        void modev(const handler_ptr &obs) override;

    };

}


#endif //WEBKISH_SELECT_POLLER_H
