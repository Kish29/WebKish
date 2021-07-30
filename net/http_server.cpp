//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "http_server.h"
#include "http_handler.h"


#include "cstring"

void http_server::on_acceptnew(int fd, const inet_address &peer_addr) {
    // todo: delete this print
    printf("new connection from %s\n", peer_addr.ip_port().c_str());
    looper.submit([=]() -> void {
        std::shared_ptr<http_handler> http_obs(new http_handler(fd));
        looper.add_observe(http_obs);
    });

    /*char buf[BUFSIZ];
    printf("read from %d\n", fd);
    ssize_t n = read(fd, buf, BUFSIZ);

    // build-body
    const char *body = "<h>this is what i send to you</h>";
    size_t bodylen = strlen(body);
    printf("New Request: %s", buf);

    // build-header
    char header[1024];
    sprintf(header, "HTTP/1.1 400 Bad Request\r\nContent-length: %lu\r\n\r\n%s", bodylen, body);
    send(fd, header, strlen(header), MSG_NOSIGNAL);
//    send(fd, header, strlen(header), 0);
    socket_utils::shutdown_write(fd);*/
}
