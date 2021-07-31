//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#include "http_interface.h"
#include "http_handler.h"

bool kish::reg_http_resolver(const http_infc_ptr &registrant) {
    if (registrant) {
        try {
            http_handler::GLOBAL_HTTP_REQUEST_RESOLVERS.insert(std::make_pair(registrant->observe_url(), registrant));
        } catch (const std::exception &e) {
            // todo:
        }
        return true;
    }
    return false;
}
