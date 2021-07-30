//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "http_server.h"
#include "http_handler.h"


void http_server::on_acceptnew(int fd, const inet_address &peer_addr) {
    // todo: delete this print
    printf("new connection from %s\n", peer_addr.ip_port().c_str());

    std::shared_ptr<http_handler> http_obs(new http_handler(fd));
    looper.submit([&]() -> void {
        looper.add_observe(http_obs);
    });
    // todo: http_server应当保存到来的连接，并根据心跳检测，设置handler的dead状态
}
