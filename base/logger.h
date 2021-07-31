//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_LOGGER_H
#define WEBKISH_LOGGER_H

#include "base.h"
#include "log_stream.h"
#include "time_stamp.h"

namespace kish {

    enum log_level {
        INFO,
        FATAL,
    };

    // 核心类，日志记录
    class logger : noncopyable {
    public:

    public:
        ~logger();

        logger(const char *filename, int line, log_level level);

    private:
        log_stream strm;
        time_stamp ts;
        log_level lv;
    };

}


#endif //WEBKISH_LOGGER_H
