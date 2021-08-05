//
// Created by 蒋澳然 on 2021/8/5.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_HEART_CHECK_H
#define WEBKISH_HTTP_HEART_CHECK_H

#include "base.h"

namespace kish {

    const static int kdefault_alive_time = 10 * 60 * 1000;  // 10 min

    class http_heart_check {
    public:
        virtual ~http_heart_check() = default;

        virtual void check_for_alive() = 0;

    };

}

#endif //WEBKISH_HTTP_HEART_CHECK_H
