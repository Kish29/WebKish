//
// Created by 蒋澳然 on 2021/7/31.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_LOG_STREAM_H
#define WEBKISH_LOG_STREAM_H

#include "sized_buffer.h"

namespace kish {

    // 提供日志的数据缓存
    class log_stream : noncopyable {
        typedef log_stream self;
    public:
        typedef sized_buffer<KSMALL_BUFFER> log_buffer;
        typedef shared_ptr<log_buffer> buffer_ptr;

        log_stream() : m_log_buf(std::make_shared<log_buffer>()) {}

    public:
        self &operator<<(char);

        self &operator<<(short);

        self &operator<<(unsigned short);

        self &operator<<(int);

        self &operator<<(unsigned int);

        self &operator<<(long);

        self &operator<<(unsigned long);

        self &operator<<(long long);

        self &operator<<(unsigned long long);

        self &operator<<(bool);

        self &operator<<(float);

        self &operator<<(double);

        self &operator<<(const char *);

        self &operator<<(const std::string &);

        self &operator<<(const unsigned char *);

        self &operator<<(const printable &);

        self &operator<<(const printable *);

    public:

        const buffer_ptr &buffer() const;

        void clear_buffer();

        void zero_buffer();

    private:
        buffer_ptr m_log_buf;

    private:
        // 格式化数字的最大位数
        static const int KMAX_NUMERIC_SIZE = 48;
    };

}

#endif //WEBKISH_LOG_STREAM_H
