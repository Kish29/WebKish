//
// Created by 蒋澳然 on 2021/8/13.
// email: 875691208@qq.com
// $desc
//

#include "file_wrapper.h"
#include "sys/stat.h"
#include "cstdio"

kish::file_wrapper::file_wrapper(const char *filename, kish::fopen_type_t t, bool abortonopenerr) : ft(t), m_filename(filename) {
    if (!filename) return;
    fptr = ::fopen(m_filename, get_fopen_mode(ft));
    if (!fptr) {
        if (abortonopenerr) {
            abort();
        }
        return;
    }
    init_stat();
}

size_t kish::file_wrapper::read_unlock(char *fbuf) const {
    if (!fbuf) return 0;
    if (ft == WRITE_ONLY_TRUNCATE || ft == APPEND) return 0;
    ::setvbuf(fptr, nullptr, _IOFBF, st_buf.st_size + 1);
    return ::fread_unlocked(fbuf, 1, st_buf.st_size + 1, fptr);
}

size_t kish::file_wrapper::read(char *fbuf) const {
    if (!fbuf) return 0;
    if (ft == WRITE_ONLY_TRUNCATE || ft == APPEND) return 0;
    ::setvbuf(fptr, nullptr, _IOFBF, st_buf.st_size + 1);
    return ::fread(fbuf, 1, st_buf.st_size + 1, fptr);
}

void kish::file_wrapper::close() {
    if (fptr) {
        ::fclose(fptr);
        fptr = nullptr;
    }
}

size_t kish::file_wrapper::get_file_size() const {
    return st_buf.st_size;
}

bool kish::file_wrapper::open(const char *filename, kish::fopen_type_t type) {
    if (!filename) return false;
    ft = type;
    this->clear();
    m_filename = filename;
    fptr = ::fopen(m_filename, get_fopen_mode(type));
    if (!fptr) return false;
    init_stat();
    return true;
}

uint64_t kish::file_wrapper::get_device_id() const {
    return st_buf.st_dev;
}

time_t kish::file_wrapper::last_access_time() const {
    return st_buf.st_atim.tv_sec;
}

time_t kish::file_wrapper::last_modify_time() const {
    return st_buf.st_mtim.tv_sec;
}

time_t kish::file_wrapper::last_status_modify() const {
    return st_buf.st_ctim.tv_sec;
}

void kish::file_wrapper::init_stat() {
    ::stat(m_filename, &st_buf);
}

void kish::file_wrapper::clear() {
    if (fptr) {
        ::fclose(fptr);
        fptr = nullptr;
        ::bzero(&st_buf, sizeof st_buf);
    }
}

const char *kish::file_wrapper::get_fopen_mode(kish::fopen_type_t type) {
    switch (type) {
        case READONLY: {
            return "rb";
        }
        case WRITE_ONLY_TRUNCATE: {
            return "w";
        }
        case READWRITE_NO_TRUNCATE: {
            return "r+";
        }
        case READWRITE_TRUNCATE: {
            return "w+";
        }
        case APPEND: {
            return "a";
        }
        case READ_APPEND: {
            return "a+";
        }
        case APPEND_CLOEXE:
            return "ae";
    }
    // unreachable here
    return "rb";
}

size_t kish::file_wrapper::write_unlock(const char *fbuf, size_t len) const {
    if (!fbuf) return 0;
    if (ft == READONLY) return 0;
    return ::fwrite_unlocked(fbuf, 1, len, fptr);
}

size_t kish::file_wrapper::write(const char *fbuf, size_t len) const {
    if (!fbuf) return 0;
    if (ft == READONLY) return 0;
    return ::fwrite(fbuf, 1, len, fptr);
}

void kish::file_wrapper::flush() {
    if (fptr) {
        ::fflush(fptr);
    }
}


