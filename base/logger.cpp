//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#include "logger.h"

class async_stream_writer : kish::noncopyable {
public:
    void print_hello() {

    }

};

#define writer kish::singleton<async_stream_writer>::instance()

void output() {

}


kish::logger::~logger() {

}

kish::logger::logger(const char *filename, int line, kish::log_level level) {
    if (filename) {
        // 用strrchr剔除绝对路径的前缀，只保留文件名
        // eg: /root/desktop/info.txt -> /info.txt
        const char *fn = strrchr(filename, '/');
        if (fn) fn++;   // 去掉 '/'
    }
}

