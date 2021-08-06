//
// Created by 蒋澳然 on 2021/8/7.
// email: 875691208@qq.com
// $desc
//

#include "server_config.h"

void server_config::setKmaxServerCnn(int kmaxServerCnn) {
    max_server_cnn = kmaxServerCnn;
}

void server_config::setHeartCheckInterval(int heartCheckInterval) {
    heart_check_interval = heartCheckInterval;
}

void server_config::setKishServerName(const std::string &kishServerName) {
    kish_server_name = kishServerName;
}

void server_config::setHttpMajorVer(uint8_t httpMajorVer) {
    http_major_ver = httpMajorVer;
}

void server_config::setHttpMinorVer(uint8_t httpMinorVer) {
    http_minor_ver = httpMinorVer;
}

int server_config::MAX_SERVER_CNN() const {
    return max_server_cnn;
}

int server_config::HEART_CHECK_INTERVAL() const {
    return heart_check_interval;
}

const std::string &server_config::KISH_SERVER_NAME() const {
    return kish_server_name;
}

uint8_t server_config::HTTP_MAJOR_VER() const {
    return http_major_ver;
}

uint8_t server_config::HTTP_MINOR_VER() const {
    return http_minor_ver;
}
