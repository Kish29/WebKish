//
// Created by 蒋澳然 on 2021/8/14.
// email: 875691208@qq.com
// $desc
//

#include "example_files_resolver.h"
#include "file_wrapper.h"

// fixme: how to release this pointer memory
__thread kish::file_wrapper *file_reader;

// todo: 如何传输大文件
bool example_files_resolver::can_resolve(const string &uri) {
    if (!file_reader) {
        file_reader = new kish::file_wrapper();
    }
    if (file_reader) {
        // 首先判断本路径下该文件是否存在
        string target_file(file_src_dir);
        // 拼接文件完整路径
        target_file.append("/");
        if (uri == "/") {	// 如果是根目录，默认打开index.html
            target_file.append("index.html");
        } else {
            target_file.append(uri);
        }
        if (file_reader->open(target_file.c_str(), kish::READONLY)) {
            // 记录当前文件的类型
            curr_file_mime_type = get_file_mime_type(target_file.c_str());
            return true;	// 返回true，表示能够解析该uri请求
        } else {
            return false;	// 返回false，表示解析失败，不能接受该请求
        }
    } else return false;
}

void example_files_resolver::on_request(const kish::http_request_ptr &, kish::http_response &response) {
    assert(file_reader);
    file_reader->assert_opened();
    // 读取文件数据
    size_t file_size = file_reader->get_file_size();
    size_t flen{};
    // fixme: 显然一次性new文件大小的内存非常危险，万一文件非常大就完蛋了
    char *fbuf = new char[file_size + 1];
    response.update_stat(200);
    flen = file_reader->read_unlock(fbuf, file_size);
    // response的contents是body数据
    response.contents.emplace_back(string(fbuf, flen));
    // 设置内容的类型，比如文本、html、图片，不设置默认是文本格式
    response.set_content_type(curr_file_mime_type);
    // 设置内容长度
    response.headers.insert(std::make_pair("Content-Length", std::to_string(flen)));
    // 关闭文件
    file_reader->close();
    delete[] fbuf;
    fbuf = nullptr;
}

