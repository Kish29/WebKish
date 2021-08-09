//
// Created by 蒋澳然 on 2021/8/7.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_MYSQL_PROXY_H
#define WEBKISH_MYSQL_PROXY_H

#include "base.h"
#include "mysql/mysql.h"
#include "sql_proxy.h"

namespace kish {

    class mysql_proxy : public sql_proxy {
    public:
        mysql_proxy();

        ~mysql_proxy() override;

        sql_result_ptr sql_query(const char *sql_query) override;

        bool sql_add(const char *sql_add) override;

        bool sql_delete(const char *sql_delete) override;

        bool sql_update(const char *sql_update) override;

        // 返回-1表示连接失败
        bool connect(const char *host, const char *user, const char *password, const char *schema, uint16_t port) override;

        void start_transaction() override;

        void end_transaction() override;

        void rollback() override;

    protected:
        MYSQL *mysql{nullptr};
    };

}


#endif //WEBKISH_MYSQL_PROXY_H
