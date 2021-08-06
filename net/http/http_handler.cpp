//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "http_handler.h"
#include "socket.h"

std::vector<shared_ptr<kish::http_interface>> kish::http_handler::GLO_RESOLR_MAPPERS;

http_handler::http_handler(int fd) : tcp_handler(fd) {
    req_parser.set_on_parse_error(std::bind(&http_handler::on_req_parse_error, this));
    req_parser.set_on_request_uri_parse_complete(std::bind(&http_handler::on_req_parse_uri_complete, this, std::placeholders::_1));
    req_parser.set_on_headers_parse_complete(std::bind(&http_handler::on_req_parse_headers_complete, this, std::placeholders::_1));
    req_parser.set_on_message_parser_complete(std::bind(&http_handler::on_message_parse_complete, this, std::placeholders::_1));
}

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

        // 1. 更新心跳时间
        latest_heart_rev_time = time_stamp();

        // 2. 解析请求
        req_parser.parse(read_buf, curr_read_len);

    }
}


// 请求解析失败
void http_handler::on_req_parse_error() {
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
}


// 请求的uri解析成功
void http_handler::on_req_parse_uri_complete(const http_request_ptr &request) {
    size_t question_mark_pos = request->uri.find_first_of('?');
    string param_str{};
    if (question_mark_pos != string::npos) {
        param_str = request->uri.substr(question_mark_pos + 1);
        request->uri = request->uri.substr(0, question_mark_pos);
    }
    // todo: 解析长uri包含的参数
    // parse_param(param_str)

    // 可以在这里查找resolver是否存在
    // 查找resolver
    if (!GLO_RESOLR_MAPPERS.empty()) {
        auto it = GLO_RESOLR_MAPPERS.begin();
        while (it != GLO_RESOLR_MAPPERS.end()) {
            if (it->get()->can_resolve(request->uri)) {
                is_404 = false;
                // 记录resolver位置
                resolver = it;
            }
            it++;
        }
    }
}

parse_control http_handler::on_req_parse_headers_complete(const http_request_ptr &request) {
    // todo: 检测首部数据
    return GO_ON;
}

void http_handler::on_message_parse_complete(const http_request_ptr &request) {
    assert(request);
// 构建response
    http_response resp = http_response_builder()
            .ver_major(1)
            .ver_minor(1)
            .header("Server", kish_server_name)
            .build();

    // 检测resolver
    if (!is_404) {
        resolver->get()->on_request(request->uri, resp);
    } else {
        resp.update_stat(404);
    }

    // 发送响应
    const string &data = resp.tomessage();
    // todo: check send len
    ::send(observe_fd, data.c_str(), data.size(), MSG_NOSIGNAL);

    // 根据请求设置是否保持连接
    alive = request->alive;
    timeout = request->timeout;

    // 更新dead标志
    if (alive == CLOSE_IMM) {
        // 关闭写端
        socket_utils::shutdown_write(observe_fd);
        i_am_dead = true;
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
