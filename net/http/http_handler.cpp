//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "http_handler.h"
#include "socket.h"

std::vector<shared_ptr<kish::http_interface>> kish::http_handler::GLO_RESOLR_MAPPERS;

void http_handler::handle_read() {
    base::handle_read();
    if (!i_am_dead && curr_read_len > 0) {
        /* 上层只负责：
         * ⚠️1、接受请求, ⚠️并更新心跳时间
         *  { a. 请求完整？
         *      ⚠️3. 解析请求，解析成功？
         *       { 是：
         *          ⚠️4. 查找已经注册resolver，有该uri的resolver？
         *             { 是
         *              a. 构建基础response对象，传给resolver，并作为最终返回报文实体
         *             }
         *             { 否
         *              b. 构建 404 Not Found，返回报文实体
         *             }
         *          ⚠️5. 根据请求的版本和header，选择是否保持连接
         *       }
         *       {
         *         否：构建 400 Bad Request，返回报文实体，关闭连接
         *       }
         *  }
         *  { b. 请求不完整
         *      ⚠️2. 持续接受请求，直到完成构建一个完整请求报文
         *  }
         */

        // 更新心跳时间
        latest_heart_rev_time = time_stamp();

        // 添加数据，解析请求
        if (curr_read_len - 1 > 0) {
            save_request.append(read_buf, curr_read_len - 1);
        }
        // 只有一个字符，认为是 CRLF "\r\n"
        save_request.append(CRLF);
        http_parser::parse_res psr_res = req_parser.parse(save_request.c_str(), save_request.size());

        switch (psr_res) {
            case http_parser::PARSE_SUCCESS: {
                // 清空暂存请求
                save_request.clear();
                // 获取解析的请求实体
                http_request_ptr req = req_parser.get_req();
                // 构建response
                http_response resp = http_response_builder()
                        .ver_major(1)
                        .ver_minor(1)
                        .header("Server", kish_server_name)
                        .build();

                bool has_resolver = false;
                // 查找resolver
                if (!GLO_RESOLR_MAPPERS.empty()) {
                    for (auto &resolver: GLO_RESOLR_MAPPERS) {
                        if (resolver->can_resolve(req->uri)) {
                            has_resolver = true;
                            resolver->on_request(req->uri, resp);
                        }
                    }
                }
                // 如果没有resolver，则构建404标志
                if (!has_resolver) {
                    resp.update_stat(404);
                }

                // 发送响应
                const string &data = resp.tomessage();
                // todo: check send len
                ::send(observe_fd, data.c_str(), data.size(), MSG_NOSIGNAL);

                // 根据请求设置是否保持连接
                alive = req->alive;
                timeout = req->timeout;

                // 更新dead标志
                if (alive == CLOSE_IMM) {
                    i_am_dead = true;
                }
                break;
            }
            case http_parser::PARSE_FAIL: {
                // 清空暂存请求
                save_request.clear();
                http_response resp = http_response_builder()
                        .ver_major(http_major_ver)
                        .ver_minor(http_minor_ver)
                        .status_code(400)
                        .header("Connection", "close")
                        .header("Server", kish_server_name)
                        .build();

                // 发送响应
                const string &data = resp.tomessage();
                // todo: check send len
                ::send(observe_fd, data.c_str(), data.size(), MSG_NOSIGNAL);
                // 关闭写端
                socket_utils::shutdown_write(observe_fd);

                // 标记该连接已死亡
                i_am_dead = true;
                break;
            }
            case http_parser::PARSE_GO_ON:
                // need do noting
                break;
        }
    }
}

bool http_handler::keep_alive() const {
    return alive == KEEP_ALIVE;
}

bool http_handler::has_set_timeout() const {
    return alive == SET_TIMEOUT;
}

void http_handler::set_dead() {
    i_am_dead = true;
}
