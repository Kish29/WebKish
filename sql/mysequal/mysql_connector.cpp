//
// Created by 蒋澳然 on 2021/8/7.
// email: 875691208@qq.com
// $desc
//

#include "mysql_connector.h"

kish::mysql_connector::mysql_connector() {
    sql = mysql_init(nullptr);
}
