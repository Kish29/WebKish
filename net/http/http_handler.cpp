//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "http_handler.h"
#include "socket.h"

std::vector <shared_ptr<kish::http_interface>> kish::http_handler::GLO_RESOLR_MAPPERS;

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

        // 2. 解析请求，解析时，每个阶段会自动调用handler的on_XXXX方法(同一个线程中)
        req_parser.parse(read_buf, curr_read_len);

    }
}


// 请求解析失败
void http_handler::on_req_parse_error() {
    http_response resp = http_response_builder()
            .ver_major(KISH_CONFIG.HTTP_MAJOR_VER())
            .ver_minor(KISH_CONFIG.HTTP_MINOR_VER())
            .status_code(400)
            .header("Connection", "close")
            .header("Server", KISH_CONFIG.KISH_SERVER_NAME())
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
    // 可以在这里查找resolver是否存在
    // 查找resolver
    has_resolver = false;
    if (!GLO_RESOLR_MAPPERS.empty()) {
        auto it = GLO_RESOLR_MAPPERS.begin();
        while (it != GLO_RESOLR_MAPPERS.end()) {
            if (it->get()->can_resolve(request->uri)) {
                has_resolver = true;
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
            .header("Server", KISH_CONFIG.KISH_SERVER_NAME())
            .header(CONTENT_TYPE_KEY, "text/plain")
            .header(CONTENT_LENGTH_KEY, "6")
            .content("Hello!")
            .build();
    resp.update_stat(200);

    // 检测resolver
    /*if (has_resolver) {
        resolver->get()->on_request(request->uri, request->params, resp);
    } else {
        resp.update_stat(404);
    }*/

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
