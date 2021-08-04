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
    if (!i_am_dead) {
        http_response resp = http_response_builder()
                .ver_major(1)
                .ver_minor(1)
                .header("Server", "Kish")
                .build();

        if (req_parser.parse(read_buf, curr_read_len)) {
            http_request_ptr req = req_parser.get_req();
            // todo: 如何查找更快？？？
            // 队头队尾
            parameters param;
            bool has_resolver = false;
            if (!GLO_RESOLR_MAPPERS.empty()) {
                for (auto &resolver: GLO_RESOLR_MAPPERS) {
                    if (resolver->can_resolve(req->uri)) {
                        has_resolver = true;
                        resolver->on_request(req->uri, resp);
                    }
                }
            }
            if (!has_resolver) {
                resp.update_stat(404);
            }
            // 根据请求设置是否保持连接
            i_am_dead = req->keep_alive;

        } else {
            resp.update_stat(400);
        }   // 默认服务端不会关闭连接

        const string &data = resp.tomessage();
        // todo: check send len
        ::send(observe_fd, data.c_str(), data.size(), MSG_NOSIGNAL);

        if (i_am_dead) {
            socket_utils::shutdown_write(observe_fd);
        }

    }
}

bool http_handler::keep_alive() const {
    return kp_alv;
}

void http_handler::set_dead() {
    i_am_dead = true;
}

