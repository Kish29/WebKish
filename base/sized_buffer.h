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

        sized_buffer() : m_buffer(new char[SIZE]), m_indexer(m_buffer) {}

        ~sized_buffer() {
            delete[] m_buffer;
        }

        void append(const char *data, size_t size) {
            size_t av = avail();
            if (av >= size) {
                memcpy(m_indexer, data, size);
                m_indexer += size;
            } else {
                memcpy(m_indexer, data, av);
                m_indexer += av;
            }
        }

        int avail() const {
            return static_cast<int>((m_buffer + SIZE) - m_indexer);
        }

        void zero_buffer() {
            // memset(m_buffer, '\0', SIZE);
            bzero(m_buffer, SIZE);
        }

        char *current() {
            return m_indexer;
        }

        void offset(size_t n) {
            m_indexer += n;
        }

        void clear() {
            // zero_buffer();   // no need to do this
            m_indexer = m_buffer;
        }

        int length() const {
            return static_cast<int>(m_indexer - m_buffer);
        }

        const char *data() {
            return m_buffer;
        }

    private:
        char *m_buffer;
        // 指向目前buffer
        char *m_indexer;
    };

}


#endif //WEBKISH_SIZED_BUFFER_H
