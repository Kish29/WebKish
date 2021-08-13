//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "http_server.h"
#include "http_interface.h"
#include "cstring"

constexpr int fbuf_size = 8 * 1024 * 1024;  // 8MB

// __thread 是线程独有的局部存储变量，不能分配8MB那么大的空间
//__thread char buf[fbuf_size];

class index_resolver : public http_resolver {
public:

    index_resolver() {
        resolver_list.emplace("/");
        resolver_list.emplace("/index.html");
        resolver_list.emplace("/favicon.icon");
        fbuf = new char[fbuf_size];
    }

    ~index_resolver() override {
        delete[] fbuf;
    }

    bool can_resolve(const string &uri) override {
        return resolver_list.find(uri) != resolver_list.end();
    }

    void on_request(const http_request_ptr &request, http_response &response) override {
        if (request->uri == "/" || request->uri == "/index.html") {
            FILE *fptr = fopen("index.html", "r");
            if (fptr) {
                response.update_stat(200);
                size_t flen = ::fread_unlocked(fbuf, 1, fbuf_size, fptr);
                response.contents.emplace_back(string(fbuf, flen));
                response.headers.insert(std::make_pair("Content-Length", std::to_string(flen)));
                response.headers.insert(std::make_pair("Content-Type", MIME_T_HTML"; charset=UTF-8"));
                ::fclose(fptr);
                fptr = nullptr;
            } else {
                response.update_stat(500);
            }
        } else if (request->uri == "/favicon.icon") {
            response.update_stat(500);
        }
    }

private:
    char *fbuf{nullptr};
    std::unordered_set<string> resolver_list;
};

int main() {
    KISH_CONFIG.setKishServerName("kishkish");
    reg_http_resolver(http_resol_ptr(new index_resolver));
    kish::http_server hs(5555, 2);
    hs.startup();
}
