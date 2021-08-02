//
// Created by 蒋澳然 on 2021/7/31.
// email: 875691208@qq.com
// $desc
//

#include "file_writer.h"

// modes为"ae"表示append，O_CLOEXE (close-on-exec)
// 具体见 man 2 open
// O_CLOEXE条目
kish::file_writer::file_writer(const char *filename) : m_fptr(::fopen(filename, "ae")) {}

kish::file_writer::~file_writer() {
    ::fclose(m_fptr);
}

void kish::file_writer::flush() {
    ::fflush(m_fptr);
}

size_t kish::file_writer::append(const char *line, size_t len) {
    size_t written = ::fwrite_unlocked(line, 1, len, m_fptr);
    size_t remain = len - written;
    while (remain > 0) {
        size_t rewrite = ::fwrite_unlocked(line + written, 1, remain, m_fptr);
        if (rewrite == 0) {
            int err = ferror(m_fptr);
            if (err) {
                // todo: delete this print
                printf("file_writer::append failed!\n");
            }
        }
        remain -= rewrite;
        written += rewrite;
    }
    return written;
}
