//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_EVENT_HANDLER_H
#define WEBKISH_EVENT_HANDLER_H

#include "base.h"

namespace kish {
    class event_handler : copyable {

    public:
        virtual void handle_event(uint32_t events) = 0;

    };
}

#endif //WEBKISH_EVENT_HANDLER_H
