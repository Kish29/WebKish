//
// Created by 蒋澳然 on 2021/8/8.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_MYSQL_CONN_POOL_H
#define WEBKISH_MYSQL_CONN_POOL_H

#include "mysql_proxy.h"
#include "atomic"
#include "sync.h"
#include "queue"

namespace kish {

    typedef std::shared_ptr<mysql_proxy> mysql_conn_ptr;

    const int KMAX_MYSQL_CONN = 6;

    class mysql_conn_pool : noncopyable {
    public:
        mysql_conn_pool() = default;

        ~mysql_conn_pool() override;

        // 初始化连接池，返回成功连接的数量
        int init(const char *host, const char *user, const char *password, const char *schema, uint16_t port, int capacity = KMAX_MYSQL_CONN);

        // sql连接池这种取出、归还的方式，一定程度上减少了并发的安全风险
        // 虽然多线程情况下，不同的connector对同一个表做修改操作也不安全就是了
        mysql_conn_ptr get_connector();

        // 归还一个连接
        void return_connector(const mysql_conn_ptr &conn);

        void close_pool();

    private:
        std::queue<mysql_conn_ptr> conns;
        mutex_lock locker;
        std::atomic_bool has_inited{false};
    };

}

#define MYSQL_POOL kish::singleton<kish::mysql_conn_pool>::instance()

#endif //WEBKISH_MYSQL_CONN_POOL_H
