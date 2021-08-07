//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_POLLER_H
#define WEBKISH_POLLER_H

#include "fdholder.h"
#include "event_handler.h"
#include "vector"

namespace kish {

    typedef shared_ptr<event_handler> handler_ptr;
    typedef std::weak_ptr<event_handler> handler_weak_ptr;
    typedef std::vector<handler_ptr> handler_list;

    class poller : public fdholder {
    public:
        virtual handler_list &poll(int timeout) = 0;

        virtual void addev(const handler_ptr &obs) = 0;

        virtual void delev(const handler_ptr &obs) = 0;

        virtual void modev(const handler_ptr &obs) = 0;

    };

}

#endif //WEBKISH_POLLER_H
