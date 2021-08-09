//
// Created by 蒋澳然 on 2021/8/8.
// email: 875691208@qq.com
// $desc
//

#include "mysql_conn_pool.h"

kish::mysql_proxy mp;

void normal_test() {
    if (mp.connect(
            "localhost",
            "jiangaoran",
            "jiangaoran",
            "mysql_basic",
            3306
    )) {
        pthread_t t[4];
//        std::shared_ptr<int> ids(new int[4]{1, 2, 3, 4});
        int *ids(new int[4]{1, 2, 3, 4});
        for (int i = 0; i < 4; ++i) {
            pthread_create(&t[i], nullptr, [](void *arg) -> void * {
                int id = *((int *) arg);
                printf("thread-%d query sql\n", id);
                auto res = mp.sql_query("select * from customers");
                if (res) {
                    printf("thread-%d query sql success\n", id);
                    for (const std::string &str: res->fields) {
                        printf("%-20s | ", str.c_str());
                    }
                    printf("\n");
                    for (const auto &row: res->rows) {
                        for (const auto &c: row) {
                            printf("%-20s | ", c.c_str());
                        }
                        printf("\n");
                    }
                    fflush(stdout);
                } else {
                    printf("thread-%d query sql failed\n", id);
                }
                pthread_exit(nullptr);
            }, (void *) (ids + i));
        }
        sleep(10);
        delete[] ids;
    }
}

void pool_test() {
    MYSQL_POOL.init(4, "localhost", "jiangaoran", "jiangaoran", "mysql_basic", 3306);
    /*kish::mysql_conn_ptr p;
    while ((p = MYSQL_POOL.get_connector()) != nullptr) {
        auto res = p->sql_query("select * from customers where cust_id=100001");
        if (res) {
            for (const std::string &str: res->fields) {
                printf("%-20s | ", str.c_str());
            }
            printf("\n");
            for (const auto &row: res->rows) {
                for (const auto &c: row) {
                    printf("%-20s | ", c.c_str());
                }
                printf("\n");
            }
            fflush(stdout);
        }
//        MYSQL_POOL.return_connector(p);
    }*/
    auto sqlp = MYSQL_POOL.get_connector();
    auto res = sqlp->sql_query("select * from customers where cust_id=100001");
    MYSQL_POOL.return_connector(sqlp);
    if (res) {
        for (const std::string &str: res->fields) {
            printf("%-10s |", str.c_str());
        }
        printf("\n");
        if (res && res->result_empty()) {
            printf("No such recorder!\n");
        }
    }

}

int main() {
//    normal_test();
    pool_test();
    return 0;
}
