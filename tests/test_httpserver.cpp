//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "http_server.h"
#include "http_interface.h"
#include "cstring"

const int file_size = 8 * 1024 * 1024;  // 8MB
__thread char buf[file_size];

class index_resolver : public http_interface {
public:

    index_resolver() {
        resolver_list.emplace("/");
        resolver_list.emplace("/index.html");
        resolver_list.emplace("/favicon.icon");
    }

    void on_request(const string &uri, http_response &response) override {
        if (uri == "/" || uri == "index.html") {
            FILE *fptr = fopen("index.html", "r");
            if (fptr) {
                response.update_stat(200);
                size_t flen = ::fread_unlocked(buf, 1, file_size, fptr);
                response.contents.emplace_back(string(buf, flen));
                response.headers.insert(std::make_pair("Content-Length", std::to_string(flen)));
                response.headers.insert(std::make_pair("Content-Type", "text/html"));
            } else {
                response.update_stat(500);
            }
            ::fclose(fptr);
            fptr = nullptr;
        } else if (uri == "/favicon.icon") {
            response.update_stat(500);
        }
    }
};

int main() {
    reg_http_interfc(http_infc_ptr(new index_resolver));
    kish::http_server hs(5555);
    hs.startup();
    SLEEP_ADAY;
}
