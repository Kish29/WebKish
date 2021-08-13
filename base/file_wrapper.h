//
// Created by 蒋澳然 on 2021/8/13.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_FILE_WRAPPER_H
#define WEBKISH_FILE_WRAPPER_H

#include "base.h"
#include "sysio_utils.h"

namespace kish {

    enum fopen_type_t {
        READONLY,
        WRITE_ONLY_TRUNCATE,
        READWRITE_NO_TRUNCATE,
        READWRITE_TRUNCATE,
        APPEND,
        // modes为"ae"表示append，O_CLOEXE (close-on-exec)
        // 具体见 man 2 open
        // O_CLOEXE条目
        APPEND_CLOEXE,
        READ_APPEND
    };

    class file_wrapper : noncopyable {
    public:

        file_wrapper() : ft(READONLY), m_filename(nullptr) {}

        explicit file_wrapper(const char *filename, fopen_type_t t = READONLY, bool abortonopenerr = false);

        ~file_wrapper() override {
            if (fptr) {
                fclose(fptr);
            }
        }

        size_t read_unlock(char *fbuf) const;

        size_t read(char *fbuf) const;

        size_t write_unlock(const char *fbuf, size_t len) const;

        size_t write(const char *fbuf, size_t len) const;

        void close();

        void flush();

        bool open(const char *filename, fopen_type_t type = READONLY);

        size_t get_file_size() const;

        uint64_t get_device_id() const;

        time_t last_access_time() const;

        time_t last_modify_time() const;

        time_t last_status_modify() const;

        void assert_opened() const { assert(fptr); }

    public:
        static const char *get_fopen_mode(fopen_type_t type);

    protected:
        fopen_type_t ft;
        const char *m_filename;
        FILE *fptr{};
        stat st_buf{};

    private:
        void init_stat();

        void clear();
    };

}


#endif //WEBKISH_FILE_WRAPPER_H
