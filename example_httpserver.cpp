//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "http_server.h"


int main() {
    kish::http_server hs(5555);
    hs.startup();
    SLEEP_ADAY;
}
