//
// Created by 蒋澳然 on 2021/8/14.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_EXAMPLE_FILES_RESOLVER_H
#define WEBKISH_EXAMPLE_FILES_RESOLVER_H

#include "http_interface.h"

class example_files_resolver : public kish::http_resolver {
public:
    explicit example_files_resolver(const char *src_dir) : file_src_dir(src_dir) {}

    bool can_resolve(const string &uri) override;

    void on_request(const kish::http_request_ptr &, kish::http_response &response) override;

private:
    const string file_src_dir;

    string curr_file_mime_type{MIME_T_TXT};
};


#endif //WEBKISH_EXAMPLE_FILES_RESOLVER_H
