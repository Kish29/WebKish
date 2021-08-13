//
// Created by 蒋澳然 on 2021/8/7.
// email: 875691208@qq.com
// $desc
//

#include "sysio_utils.h"
#include "http_server.h"
#include "http_interface.h"
#include "algorithm"
#include "mysql_conn_pool.h"
#include "file_wrapper.h"

constexpr int fbuf_size = 8 * 1024 * 1024;  // 8MB

// todo: 如何传输大文件
class file_resolver : public kish::http_resolver {
public:

    explicit file_resolver(const char *src_dir) : file_src_dir(src_dir) {
        fbuf = new char[fbuf_size];
    }

    ~file_resolver() override {
        delete[] fbuf;
    }

    bool can_resolve(const string &uri) override {
        // 首先判断本路径下该文件是否存在
        string target_file(file_src_dir);
        target_file.append("/");
        if (uri == "/") {
            target_file.append("index.html");
        } else {
            target_file.append(uri);
        }
        if (file.open(target_file.c_str(), READONLY)) {
            curr_file_mime_type = get_file_mime_type(target_file.c_str());
            return true;
        } else {
            return false;
        }
    }

    void on_request(const http_request_ptr &request, http_response &response) override {
        file.assert_opened();
        size_t file_size = file.get_file_size();
        size_t flen{};
        if (file_size < fbuf_size) {
            response.update_stat(200);
            flen = file.read_unlock(fbuf, file_size);
        } else {
            response.update_stat(206);
            flen = file.read_unlock(fbuf, fbuf_size);
        }
        response.contents.emplace_back(string(fbuf, flen));
        response.set_content_type(curr_file_mime_type);
        response.headers.insert(std::make_pair("Content-Length", std::to_string(flen)));
        file.close();
    }

private:
    file_wrapper file{};
    const string file_src_dir;

    char *fbuf{nullptr};
    string curr_file_mime_type{MIME_T_TXT};
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

#include "iostream"

class user_interface : public http_interface {
public:

    void on_reg_in(http_interface_holder &holder) override {
        holder.regin("/users/login", infc_type_t{
                .infc = std::bind(&user_interface::user_login, this, std::placeholders::_1, std::placeholders::_2),
                .method = HTTP_POST});
        holder.regin("/users/reg", infc_type_t{
                .infc = std::bind(&user_interface::user_regis, this, std::placeholders::_1, std::placeholders::_2),
                .method = HTTP_POST});
    }

    void user_login(const http_request_ptr &request, http_response &response) {
        response.update_stat(200);
    }

    void user_regis(const http_request_ptr &request, http_response &response) {
        const string &type = request->get_content_type();
        if (type == MIME_A_JSON) {
            try {
                auto param_json = (CJsonObject) request->get_param(http_message::JSON_PARAM);
                string password;
                param_json.Get("password", password);
                std::cout << password << std::endl;
                string username;
                param_json.Get("username", username);
                std::cout << username << std::endl;
                int index = 0;
                string key;
                string been;
                for (int i = 0; i < param_json["file_list"].GetArraySize(); ++i) {
                    key = "file" + std::to_string(++index);
                    param_json["file_list"][i].Get(key, been);
                    std::cout << been << std::endl;
                    key = "file" + std::to_string(++index);
                    param_json["file_list"][i].Get(key, been);
                    std::cout << been << std::endl;
                }
                response.update_stat(200);
            } catch (const std::bad_cast &e) {
                response.update_stat(400);
            }
        } else {
            try {
                string username = (string) request->get_param("username");
                string password = (string) request->get_param("password");
                std::cout << "username: " << username << '\n';
                std::cout << "password: " << password << '\n';
                response.update_stat(200);
            } catch (const std::bad_cast &e) {
                response.update_stat(400);
            }
        }
    }

private:

};

int main(int argc, char *argv[]) {
//    map_test();
//    saver_test();
//    FILE *p = fopen("/tmp/hhh", "rb");
//    if (p) {
//        printf("success");
//    } else {
//        printf("failed");
//    }

    /*MYSQL_POOL.init(
            "localhost",
            "jiangaoran",
            "jiangaoran",
            "webkish",
            3306);*/
    reg_http_resolver(http_resol_ptr(new file_resolver("/home/parallels/webserver/static_docs")));
    reg_http_interface(http_intc_ptr(new user_interface));
    http_server hs(5555, 2);
    hs.startup();
    return 0;
}
