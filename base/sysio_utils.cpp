//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "sysio_utils.h"
#include "cstdio"
#include "dirent.h"
#include "sys/stat.h"
#include "logger.h"

bool enable_nonblock_and_cloexec(int fd) {
    int flags = ::fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int res = ::fcntl(fd, F_SETFL, flags);
    if (res == -1) {
#ifdef __DEBUG__
        printf("set fd[%d] O_NONBLOCK failed!", fd);
#endif
        return false;
    }
    flags = ::fcntl(fd, F_GETFL, 0);
    flags |= O_CLOEXEC;
    res = ::fcntl(fd, F_SETFL, flags);
    if (res == -1) {
#ifdef __DEBUG__
        printf("set fd[%d] O_CLOEXEC failed!", fd);
#endif
        return false;
    }
#ifdef __DEBUG__
    printf("set non-block for %d success!\n", fd);
#endif
    return true;
}


#define PATH_BUF_SIZE  1024
#define CURR_DIR    "."
#define BACK_DIR    ".."

#define DEFAULT_TYPE_SIZE 256

inline void map_insert(file_mapper &mapper, const char *suffix, const char *base_path) {
    if (mapper.find(suffix) == mapper.end()) {
        mapper.emplace(std::make_pair(suffix, std::vector<std::string>()));
        mapper.at(suffix).reserve(DEFAULT_TYPE_SIZE);
    }
    mapper.at(suffix).emplace_back(base_path);   // 有后缀加入到相应的位置
}

void readdir_sort_by_suffix(const char *base_path, file_mapper &mapper, extra_filter filter, bool filter_hidden) {
    if (!base_path) return;
    if (filter && filter(base_path)) return;  // 如果过滤掉，返回
    struct stat st_buf{};
    stat(base_path, &st_buf);
    if (S_ISREG(st_buf.st_mode)) {      // 如果是普通文件
        const char *sfx = strrchr(base_path, '.');  // 得到后缀
        map_insert(mapper, sfx ? sfx : TEXT, base_path);
    } else if (S_ISDIR(st_buf.st_mode)) {
        DIR *dir_ptr = opendir(base_path);
        if (!dir_ptr) {
            LOG_INFO << "opendir() fatal error! abort!";
            abort();
        }
        struct dirent *f{};
        char path_buf[PATH_BUF_SIZE];
        while ((f = readdir(dir_ptr)) != nullptr) {
            if (strcmp(f->d_name, CURR_DIR) == 0 || strcmp(f->d_name, BACK_DIR) == 0) {
                continue;
            }
            if (f->d_name[0] == '.' && strlen(base_path) > 2 && filter_hidden) { //或者是隐藏文件
                continue;
            }
            bzero(path_buf, sizeof path_buf);
            strcat(path_buf, base_path);
            size_t path_len = strlen(base_path);
            if (base_path[path_len - 1] != '/') {
                strcat(path_buf, "/");
            }
            strcat(path_buf, f->d_name);
            readdir_sort_by_suffix(path_buf, mapper, filter, filter_hidden);
        }
        closedir(dir_ptr);
    }
}

void readdir_and_save(const char *base_path, std::vector<std::string> &saver, extra_filter filter, bool filter_hidden) {
    if (!base_path) return;
    if (filter && filter(base_path)) return;
    struct stat st_buf{};
    stat(base_path, &st_buf);
    if (S_ISREG(st_buf.st_mode)) {
        saver.emplace_back(base_path);
    } else if (S_ISDIR(st_buf.st_mode)) {
        DIR *dir_ptr = opendir(base_path);
        if (!dir_ptr) {
            LOG_INFO << "opendir() fatal error! abort!";
            abort();
        }
        struct dirent *f{};
        char path_buf[PATH_BUF_SIZE];
        while ((f = ::readdir(dir_ptr)) != nullptr) {
            if (strcmp(f->d_name, CURR_DIR) == 0 || strcmp(f->d_name, BACK_DIR) == 0) {
                continue;
            }
            if (f->d_name[0] == '.' && strlen(base_path) > 2 && filter_hidden) { //或者是隐藏文件
                continue;
            }
            bzero(path_buf, sizeof path_buf);
            strcat(path_buf, base_path);
            size_t path_len = strlen(base_path);
            if (base_path[path_len - 1] != '/') {
                strcat(path_buf, "/");
            }
            strcat(path_buf, f->d_name);
            readdir_and_save(path_buf, saver, filter, filter_hidden);
        }
        closedir(dir_ptr);
    }
}
