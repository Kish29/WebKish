//
// Created by 蒋澳然 on 2021/7/31.
// email: 875691208@qq.com
// $desc
//

#include "file_writer.h"

size_t kish::file_writer::append(const char *line, size_t len) {
    size_t written = ::fwrite_unlocked(line, 1, len, fptr);
    size_t remain = len - written;
    while (remain > 0) {
        size_t rewrite = ::fwrite_unlocked(line + written, 1, remain, fptr);
        if (rewrite == 0) {
            // todo: 完善判断条件
            int err = ferror(fptr);
            if (err) {
                perror("log system internal fatal error occurred!\n");
                abort();
            }
        }
        remain -= rewrite;
        written += rewrite;
    }
    return written;
}
