//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_BASE_H
#define WEBKISH_BASE_H

#include "functional"
#include "memory"
#include "unistd.h"

// 86400 = 60 * 60 * 24
#define SLEEP_ADAY sleep(86400)

namespace kish {

    using std::function;
    using std::shared_ptr;

    typedef function<void(void *)> callback;
    typedef shared_ptr<callback> callback_ptr;

    class noncopyable {
    public:
        noncopyable(const noncopyable &) = delete;

        noncopyable &operator=(const noncopyable &) = delete;

    protected:
        noncopyable() = default;

        ~noncopyable() = default;
    };

    class copyable {
    protected:
        copyable() = default;

        ~copyable() = default;

    };

}

#endif //WEBKISH_BASE_H
