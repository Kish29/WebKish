//
// Created by 蒋澳然 on 2021/7/31.
// email: 875691208@qq.com
// $desc
//

#include "log_stream.h"

kish::log_stream::self &kish::log_stream::operator<<(char val) {
    log_buf->append(&val, 1);
    return *this;
}

kish::log_stream::self &kish::log_stream::operator<<(short val) {
    return *this << static_cast<int>(val);
}

kish::log_stream::self &kish::log_stream::operator<<(int val) {
    if (log_buf->avail() >= KMAX_NUMERIC_SIZE) {
        char buf[12];   // -2147483648 11位
        sprintf(buf, "%d", val);
        log_buf->append(buf, strlen(buf));
    }
    return *this;
}

kish::log_stream::self &kish::log_stream::operator<<(unsigned short val) {
    return *this << static_cast<int>(val);
}

kish::log_stream::self &kish::log_stream::operator<<(unsigned int val) {
    if (log_buf->avail() >= KMAX_NUMERIC_SIZE) {
        char buf[12];
        sprintf(buf, "%u", val);
        log_buf->append(buf, strlen(buf));
    }
    return *this;
}

kish::log_stream::self &kish::log_stream::operator<<(long val) {
    if (log_buf->avail() >= KMAX_NUMERIC_SIZE) {
        char buf[24];
        sprintf(buf, "%ld", val);
        log_buf->append(buf, strlen(buf));
    }
    return *this;
}

kish::log_stream::self &kish::log_stream::operator<<(unsigned long val) {
    if (log_buf->avail() >= KMAX_NUMERIC_SIZE) {
        char buf[24];
        sprintf(buf, "%lu", val);
        log_buf->append(buf, strlen(buf));
    }
    return *this;
}

kish::log_stream::self &kish::log_stream::operator<<(long long int val) {
    if (log_buf->avail() >= KMAX_NUMERIC_SIZE) {
        char buf[24];
        sprintf(buf, "%lld", val);
        log_buf->append(buf, strlen(buf));
    }
    return *this;
}

kish::log_stream::self &kish::log_stream::operator<<(unsigned long long int val) {
    if (log_buf->avail() >= KMAX_NUMERIC_SIZE) {
        char buf[24];
        sprintf(buf, "%llu", val);
        log_buf->append(buf, strlen(buf));
    }
    return *this;
}

kish::log_stream::self &kish::log_stream::operator<<(bool val) {
    if (val) {
        log_buf->append("true", 4);
    } else {
        log_buf->append("false", 5);
    }
    return *this;
}

kish::log_stream::self &kish::log_stream::operator<<(float val) {
    return *this << static_cast<double >(val);
}

kish::log_stream::self &kish::log_stream::operator<<(double val) {
    if (log_buf->avail() >= KMAX_NUMERIC_SIZE) {
        int len = snprintf(log_buf->current(), KMAX_NUMERIC_SIZE, "%.12g", val);
        log_buf->offset(len);
    }
    return *this;
}


kish::log_stream::self &kish::log_stream::operator<<(const char *val) {
    if (val) {
        log_buf->append(val, strlen(val));
    } else {
        log_buf->append("(null)", 6);
    }
    return *this;
}

kish::log_stream::self &kish::log_stream::operator<<(const std::string &val) {
    if (!val.empty()) {
        return *this << val.c_str();
    }
    return *this;
}

kish::log_stream::self &kish::log_stream::operator<<(const unsigned char *val) {
    return *this << reinterpret_cast<const char *>(val);
}

kish::log_stream::self &kish::log_stream::operator<<(const kish::printable &val) {
    return *this << val.tostring();
}

kish::log_stream::self &kish::log_stream::operator<<(const kish::printable *val) {
    if (val) {
        return *this << val->tostring();
    } else {
        return *this << "[null object]";
    }
}

const kish::log_stream::buffer_ptr &kish::log_stream::buffer() const {
    return log_buf;
}

void kish::log_stream::clear_buffer() {
    log_buf->clear();
}

void kish::log_stream::zero_buffer() {
    log_buf->zero_buffer();
}
