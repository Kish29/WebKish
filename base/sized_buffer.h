//
// Created by 蒋澳然 on 2021/7/31.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_SIZED_BUFFER_H
#define WEBKISH_SIZED_BUFFER_H

#include "base.h"
#include "cstring"

namespace kish {

    const int KSMALL_BUFFER = 4 * 1024; // 4KB
    const int KLARGE_BUFFER = 4 * 1024 * 1024;  // 4MB

    // 固定大小的buffer，缓冲区为堆内申请
    template<int SIZE>
    class sized_buffer : noncopyable {
    public:

        sized_buffer() : buffer(new char[SIZE]), indexer(buffer) {}

        ~sized_buffer() override {
            delete[] buffer;
            buffer = nullptr;
            indexer = nullptr;
        }

        void append(const char *data, size_t size) {
            size_t av = avail();
            if (av >= size) {
                memcpy(indexer, data, size);
                indexer += size;
            } else {
                memcpy(indexer, data, av);
                indexer += av;
            }
        }

        int avail() const {
            return static_cast<int>((buffer + SIZE) - indexer);
        }

        void zero_buffer() {
            // memset(buffer, '\0', SIZE);
            bzero(buffer, SIZE);
        }

        char *current() {
            return indexer;
        }

        void offset(size_t n) {
            indexer += n;
        }

        void clear() {
            // zero_buffer();   // no need to do this
            indexer = buffer;
        }

        int length() const {
            return static_cast<int>(indexer - buffer);
        }

        const char *data() {
            return buffer;
        }

    private:
        char *buffer;
        // 指向目前buffer
        char *indexer;
    };

}


#endif //WEBKISH_SIZED_BUFFER_H
