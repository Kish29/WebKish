//
// Created by 蒋澳然 on 2021/8/7.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_SERVER_CONFIG_H
#define WEBKISH_SERVER_CONFIG_H

#include "base.h"

class server_config;

#define KISH_CONFIG kish::singleton<server_config>::instance()

// todo: 将一些配置放在该类中
// todo: 该类读取配置文件，解析配置
class server_config : kish::noncopyable {
public:
    int MAX_SERVER_CNN() const;

    int HEART_CHECK_INTERVAL() const;

    const std::string &KISH_SERVER_NAME() const;

    uint8_t HTTP_MAJOR_VER() const;

    uint8_t HTTP_MINOR_VER() const;

    void setKmaxServerCnn(int kmaxServerCnn);

    void setHeartCheckInterval(int heartCheckInterval);

    void setKishServerName(const std::string &kishServerName);

    void setHttpMajorVer(uint8_t httpMajorVer);

    void setHttpMinorVer(uint8_t httpMinorVer);

private:
// 服务器的最大并发连接数量
    int max_server_cnn{100000};
    int heart_check_interval{10};    // 10ms
// 服务器名称，修改此字段更改
    std::string kish_server_name{"Kish"};
// 服务器HTTP版本号
    uint8_t http_major_ver{1};
    uint8_t http_minor_ver{1};

};

#endif //WEBKISH_SERVER_CONFIG_H
