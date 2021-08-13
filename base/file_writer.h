//
// Created by 蒋澳然 on 2021/7/31.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_FILE_WRITER_H
#define WEBKISH_FILE_WRITER_H

#include "base.h"
#include "file_wrapper.h"

namespace kish {

    class file_writer : public file_wrapper {
    public:
        explicit file_writer(const char *filename) : file_wrapper(filename, APPEND_CLOEXE, true) {}

        size_t append(const char *line, size_t len);

    };
}


#endif //WEBKISH_FILE_WRITER_H
