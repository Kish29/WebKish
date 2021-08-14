//
// Created by 蒋澳然 on 2021/8/14.
// email: 875691208@qq.com
// $desc
//

#include "example_files_resolver.h"
#include "example_users_interface.h"

#include "http_server.h"

int main() {
    KISH_CONFIG.setKishServerName("example_webserver");    // example：更改服务器名称

    // 注册resolver
    reg_http_resolver(http_resol_ptr(
            new example_files_resolver("/home/parallels/webserver/static_docs")));

    // 注册用户接口
    reg_http_interface(http_intc_ptr(new example_users_interface));
    // 开启8个looper（即8个epoll fd）
    kish::http_server hs(7777, 8);    // 创建服务器，工作在端口7777，默认地址0.0.0.0

    hs.startup();        // 运行服务器
}
