//
// Created by 蒋澳然 on 2021/8/7.
// email: 875691208@qq.com
// $desc
//

#include "mysql_proxy.h"
#include "logger.h"
#include "mysql/errmsg.h"

kish::mysql_proxy::mysql_proxy() {
    mysql = ::mysql_init(nullptr);
    if (!mysql) {
        LOG_FATAL << "mysql init failed!";
        abort();
    }
    // 设置字符编码集
    ::mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "utf8");
}

kish::sql_result_ptr kish::mysql_proxy::sql_query(const char *sql_query) {
    if (sql_query == nullptr) {
        error_reason = INVALID_PARAM;
        return nullptr;
    }
    if (::mysql_query(mysql, sql_query) != -1) {
        std::shared_ptr<sql_result_store> res = std::make_shared<sql_result_store>();
        MYSQL_RES *sql_res = mysql_store_result(mysql);
        if (sql_res) {
            res->fields.reserve(sql_res->field_count);
            res->rows.reserve(sql_res->row_count);
            for (int i = 0; i < sql_res->field_count; ++i) {
                res->fields.emplace_back(sql_res->fields[i].name);
            }
            // 数据为空
            if (sql_res->row_count == 0) {
                mysql_free_result(sql_res);
                return res;
            }
            MYSQL_ROW row{};
            while ((row = mysql_fetch_row(sql_res))) {
                res->rows.emplace_back();
                res->rows.back().reserve(sql_res->field_count);
                for (int i = 0; i < sql_res->field_count; ++i) {
                    if (row[i]) {
                        res->rows.back().emplace_back(row[i]);
                    }
                }
            }
            mysql_free_result(sql_res);
            error_reason = SUCCESS;
            return res;
        } else {
            error_reason = SUCCESS;
            return nullptr;
        }
    } else {
        error_reason = OPERATOR_FAIL;
        return nullptr;
    }
}

kish::mysql_proxy::~mysql_proxy() {
    ::mysql_close(mysql);
}

bool kish::mysql_proxy::sql_insert(const char *sql_insert) {
    if (!sql_insert) {
        error_reason = INVALID_PARAM;
        return false;
    }
    return ::mysql_query(mysql, sql_insert) == 0;       // mysql api 返回值为0表示成功
}

bool kish::mysql_proxy::sql_delete(const char *sql_delete) {
    if (!sql_delete) {
        error_reason = INVALID_PARAM;
        return false;
    }
    return ::mysql_query(mysql, sql_delete) == 0;
}

bool kish::mysql_proxy::sql_update(const char *sql_update) {
    if (!sql_update) {
        error_reason = INVALID_PARAM;
        return false;
    }
    return ::mysql_query(mysql, sql_update) == 0;
}

bool kish::mysql_proxy::connect(const char *host, const char *user, const char *password, const char *schema, uint16_t port) {
    if (!host || !user || !password || !schema) {
        error_reason = INVALID_PARAM;
        return false;
    }
    if (::mysql_real_connect(
            mysql,
            host,
            user,
            password,
            schema,
            port,
            nullptr,
            0
    ) == nullptr) {
        switch (::mysql_errno(mysql)) {
            case CR_SOCKET_CREATE_ERROR:
                error_reason = INTERNAL;
                break;
            case CR_CONNECTION_ERROR:
            case CR_CONN_HOST_ERROR:
                error_reason = CONN_FAIL;
                break;
            default:
                error_reason = UNKNOWN;
                break;
        }
    }
    return true;
}

void kish::mysql_proxy::start_transaction() {

}

void kish::mysql_proxy::end_transaction() {

}

void kish::mysql_proxy::rollback() {

}
