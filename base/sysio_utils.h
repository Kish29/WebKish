//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_SYSIO_UTILS_H
#define WEBKISH_SYSIO_UTILS_H

#include "fcntl.h"
#include "cstring"
#include "string"
#include "vector"
#include "map"

bool enable_nonblock_and_cloexec(int fd);

#define TEXT    ".txt"  // 无后缀的文件将默认保存在txt分类下

typedef std::string suffix;
typedef std::string file_path;
typedef std::map<suffix, std::vector<file_path>> file_mapper;

// 额外的过滤规则，返回true表示过滤掉该文件夹/文件
typedef bool (*extra_filter)(const char *f_d_name);

void readdir_sort_by_suffix(const char *base_path, file_mapper &mapper, extra_filter filter = nullptr, bool filter_hidden = true);

void readdir_and_save(const char *base_path, std::vector<std::string> &saver, extra_filter filter = nullptr, bool filter_hidden = true);

#endif //WEBKISH_SYSIO_UTILS_H
