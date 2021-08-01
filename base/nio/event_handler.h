//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_EVENT_HANDLER_H
#define WEBKISH_EVENT_HANDLER_H

#include "base.h"
#include "fdholder.h"

namespace kish {

    class event_handler : public fdholder, copyable {
    public:

        virtual void handle_event(uint32_t events) = 0;

        virtual uint32_t events() const = 0;

        virtual void update_latest_events(uint32_t levents) = 0;

    };

}

#endif //WEBKISH_EVENT_HANDLER_H
