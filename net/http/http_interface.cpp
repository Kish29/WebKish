//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#include "http_interface.h"
#include "http_handler.h"

static pthread_once_t ponce = PTHREAD_ONCE_INIT;

bool kish::reg_http_interfc(const http_infc_ptr &registrant) {
    pthread_once(&ponce, []() -> void {
        kish::http_handler::GLO_RESOLR_MAPPERS.reserve(kish::http_handler::KDEFAULT_RESOLVER_SIZE);
    });
    try {
        kish::http_handler::GLO_RESOLR_MAPPERS.emplace_back(registrant);
    } catch (const std::exception &e) {
        // todo: error will occur?
    }
    return false;
}
