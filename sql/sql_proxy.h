//
// Created by 蒋澳然 on 2021/8/8.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_SQL_PROXY_H
#define WEBKISH_SQL_PROXY_H

#include "base.h"
#include "vector"

namespace kish {

    struct sql_result_store {
        std::vector<std::string> fields{};
        std::vector<std::vector<std::string>> rows{};

        bool result_empty() const { return rows.empty(); };
    };

    typedef std::shared_ptr<sql_result_store> sql_result_ptr;

    enum proxy_error_t {
        CONN_FAIL,
        INVALID_PARAM,
        OPERATOR_FAIL,  // 操作数据库出错
        INTERNAL,   // 系统内部出错
        UNKNOWN
    };

    class sql_proxy : public noncopyable {
    public:

        virtual sql_result_ptr sql_query(const char *sql_query) = 0;

        virtual bool sql_add(const char *sql_add) = 0;

        virtual bool sql_delete(const char *sql_delete) = 0;

        virtual bool sql_update(const char *sql_update) = 0;

        virtual bool connect(
                const char *host,
                const char *user,
                const char *password,
                const char *schema,
                uint16_t port
        ) = 0;

        // 上面的原子操作都仅仅返回boo类型，如果要判断错误类型
        // 在每次调用操作后使用该接口
        virtual proxy_error_t get_error_reason() {
            return error_reason;
        }

        // 开启事务
        virtual void start_transaction() = 0;

        virtual void end_transaction() = 0;

        virtual void rollback() = 0;

    protected:
        proxy_error_t error_reason{UNKNOWN};

    };

}

#endif //WEBKISH_SQL_PROXY_H
