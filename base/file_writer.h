//
// Created by 蒋澳然 on 2021/7/31.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_FILE_WRITER_H
#define WEBKISH_FILE_WRITER_H

#include "base.h"

namespace kish {

    class file_writer : noncopyable {
    public:
        explicit file_writer(const char *filename);

        ~file_writer();

        size_t append(const char *line, size_t len);

        void flush();

    private:
        FILE *m_fptr;

    };
}


#endif //WEBKISH_FILE_WRITER_H
