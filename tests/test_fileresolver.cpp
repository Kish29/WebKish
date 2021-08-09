//
// Created by 蒋澳然 on 2021/8/7.
// email: 875691208@qq.com
// $desc
//

#include "sysio_utils.h"
#include "http_server.h"
#include "http_interface.h"
#include "algorithm"

constexpr int file_size = 8 * 1024 * 1024;  // 8MB

class file_resolver : public kish::http_interface {
public:
    file_resolver() {
        fbuf = new char[file_size];
    }

    ~file_resolver() override {
        delete[] fbuf;
        fclose(curr_fptr);
    }

    bool can_resolve(const string &uri) override {
        if (uri == "/") return (curr_fptr = ::fopen("index.html", "rb")) != nullptr;
        // 首先判断本路径下该文件是否存在
        string target_file(file_dir);
        target_file.append("/").append(uri);
        curr_fptr = ::fopen(target_file.c_str(), "rb");
        return curr_fptr != nullptr;
    }

    void on_request(const string &uri, const param_container &params, http_response &response) override {
        assert(curr_fptr);
        response.update_stat(200);
        setvbuf(curr_fptr, nullptr, _IOFBF, file_size);
        size_t flen = ::fread_unlocked(fbuf, 1, file_size, curr_fptr);
        response.contents.emplace_back(string(fbuf, flen));
        response.headers.insert(std::make_pair("Content-Length", std::to_string(flen)));
        response.headers.insert(std::make_pair("Content-Type", MIME_TXT"; charset=UTF-8"));
        ::fclose(curr_fptr);
        curr_fptr = nullptr;
    }

private:
    FILE *curr_fptr{nullptr};
    const string file_dir{"/home/parallels/webserver/static_docs"};

    char *fbuf{nullptr};
};

void map_test() {
    printf("------------- map test ---------------\n");
    file_mapper mapper;
    const char *path = "/tmp/webkish";
    readdir_sort_by_suffix(path, mapper, [](const char *f_d_name) -> bool {
        if (strcmp(f_d_name, "/tmp/webkish/cmake-build-debug") == 0 || strcmp(f_d_name, "/tmp/webkish/cmake-build-debug-fedora") == 0) {
            return true;
        } else return false;
    });

    for (auto const &fs: mapper) {
        printf("suffix is: %s\n", fs.first.c_str());
        for (const string &fn: fs.second) {
            printf("%s\n", fn.c_str());
        }
        printf("\n");
    }
}

void saver_test() {
    printf("------------- saver test ---------------\n");
    std::vector<string> saver;
    saver.reserve(200);
    const char *path = "/tmp/webkish";
    readdir_and_save(path, saver, [](const char *f_d_name) -> bool {
        if (strcmp(f_d_name, "/tmp/webkish/cmake-build-debug") == 0 || strcmp(f_d_name, "/tmp/webkish/cmake-build-debug-fedora") == 0) {
            return true;
        } else return false;
    });

    for (auto const &fn: saver) {
        printf("%s\n", fn.c_str());
    }
}

int main(int argc, char *argv[]) {
//    map_test();
//    saver_test();
//    FILE *p = fopen("/tmp/hhh", "rb");
//    if (p) {
//        printf("success");
//    } else {
//        printf("failed");
//    }
    reg_http_interfc(http_infc_ptr(new file_resolver));
    http_server hs(5555, 2);
    hs.startup();
    return 0;
}
