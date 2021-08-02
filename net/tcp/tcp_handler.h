//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_TCP_HANDLER_H
#define WEBKISH_TCP_HANDLER_H

#include "epoll_handler.h"

namespace kish {

    class tcp_handler : public epoll_handler {
    public:
        explicit tcp_handler(int fd);

        uint32_t events() const override;

        void handle_read() override;

    protected:
        char m_send_buf[KSEND_BUFSIZ]{};
    };
}


#endif //WEBKISH_TCP_HANDLER_H
