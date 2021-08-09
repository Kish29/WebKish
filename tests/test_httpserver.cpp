//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "http_server.h"
#include "http_interface.h"
#include "cstring"

constexpr int file_size = 8 * 1024 * 1024;  // 8MB

// __thread 是线程独有的局部存储变量，不能分配8MB那么大的空间
//__thread char buf[file_size];

class index_resolver : public http_interface {
public:

    index_resolver() {
        resolver_list.emplace("/");
        resolver_list.emplace("/index.html");
        resolver_list.emplace("/favicon.icon");
        fbuf = new char[file_size];
    }

    ~index_resolver() override {
        delete[] fbuf;
    }

    void on_request(const string &uri, const param_container &params, http_response &response) override {
        if (uri == "/" || uri == "/index.html") {
            FILE *fptr = fopen("index.html", "r");
            if (fptr) {
                response.update_stat(200);
                size_t flen = ::fread_unlocked(fbuf, 1, file_size, fptr);
                response.contents.emplace_back(string(fbuf, flen));
                response.headers.insert(std::make_pair("Content-Length", std::to_string(flen)));
                response.headers.insert(std::make_pair("Content-Type", MIME_HTML"; charset=UTF-8"));
                ::fclose(fptr);
                fptr = nullptr;
            } else {
                response.update_stat(500);
            }
        } else if (uri == "/favicon.icon") {
            response.update_stat(500);
        }
    }

private:
    char *fbuf{nullptr};
};

int main() {
    KISH_CONFIG.setKishServerName("kishkish");
    reg_http_interfc(http_infc_ptr(new index_resolver));
    kish::http_server hs(5555, 2);
    hs.startup();
}
