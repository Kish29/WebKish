//
// Created by 蒋澳然 on 2021/8/7.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_MYSQL_CONNECTOR_H
#define WEBKISH_MYSQL_CONNECTOR_H

#include "base.h"
#include "mysql/mysql.h"

namespace kish {

    class mysql_connector : noncopyable {
    public:
        mysql_connector();

    protected:
        MYSQL *sql{nullptr};

    };

}


#endif //WEBKISH_MYSQL_CONNECTOR_H
