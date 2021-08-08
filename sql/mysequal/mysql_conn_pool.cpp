//
// Created by 蒋澳然 on 2021/8/8.
// email: 875691208@qq.com
// $desc
//

#include "mysql_conn_pool.h"

#include <memory>
#include "kish_utils.h"

kish::mysql_conn_pool::~mysql_conn_pool() {
    close_pool();
}

int kish::mysql_conn_pool::init(int capacity, const char *host, const char *user, const char *password, const char *schema, uint16_t port) {
    if (has_inited) return (int) conns.size();
    has_inited = true;
    capacity = coerce_in(capacity, 1, KMAX_MYSQL_CONN);
    for (int i = 0; i < capacity; ++i) {
        conns.emplace(std::make_shared<mysql_proxy>());
        if (!conns.back()->connect(
                host,
                user,
                password,
                schema,
                port
        )) {
            conns.pop();
        }
    }
    return (int) conns.size();
}

void kish::mysql_conn_pool::close_pool() {
    while (!conns.empty()) {
        conns.pop();
    }
}

kish::mysql_conn_ptr kish::mysql_conn_pool::get_connector() {
    if (conns.empty()) return nullptr; // 如果没有空闲
    // 获取连接对象
    mutex_lockguard lck(locker);
    mysql_conn_ptr p = conns.front();
    conns.pop();
    return p;
}

void kish::mysql_conn_pool::return_connector(const kish::mysql_conn_ptr &conn) {
    mutex_lockguard lck(locker);
    conns.emplace(conn);
}
