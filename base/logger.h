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

    // 加LL前缀的原因是名字和其他的enum类型名字冲突了。。。。
    enum log_level {
        LL_TRACE,       // 输出屏幕+文件
        LL_DEBUG,       // 仅输出屏幕
        LL_INFO,        // 仅输出屏幕
        LL_WARN,        // 输出屏幕+文件
        LL_FATAL,       // 输出屏幕+文件
        LL_RECOR,       // 仅输出文件
        LL_LEVEL_NUM
    };

    // fixme: 确保在所有LOG调用前初始化
    void set_logconf(log_level level, const char *filename);

    // 核心类，日志记录
    class logger : noncopyable {
    public:
        // 在析构函数中，strm中的数据输入到文件中
        ~logger();

        logger(const char *filename, int line, log_level level);

        log_stream &stream() {
            return strm;
        }

    private:
        const char *filnm;
        int ln;
        log_stream strm{};
        time_stamp ts{};
        log_level lv;
        int saved_errno;

    private:
        void weedout_filname_slash();

        void append_logpos();

        static const char *level_str[LL_LEVEL_NUM];
    };

}

#define LOG_TRACE kish::logger(__FILE__,__LINE__,kish::LL_TRACE).stream()
#define LOG_DEBUG kish::logger(__FILE__,__LINE__,kish::LL_DEBUG).stream()
#define LOG_INFO kish::logger(__FILE__,__LINE__,kish::LL_INFO).stream()
#define LOG_WARN kish::logger(__FILE__,__LINE__,kish::LL_WARN).stream()
#define LOG_FATAL kish::logger(__FILE__,__LINE__,kish::LL_FATAL).stream()
#define LOG_RECOR kish::logger(__FILE__,__LINE__,kish::LL_RECOR).stream()


#endif //WEBKISH_LOGGER_H
