//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_HANDLER_H
#define WEBKISH_HTTP_HANDLER_H

#include "tcp_handler.h"

namespace kish {

    class http_handler : public tcp_handler {
        typedef tcp_handler base;
    public:

        explicit http_handler(int fd) : tcp_handler(fd) {}

        void handle_read() override;

        bool keep_alive() const;

        void set_dead();

    private:
        bool kp_alv{false};
    };
}


#endif //WEBKISH_HTTP_HANDLER_H
