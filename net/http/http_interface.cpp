//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#include "http_interface.h"
#include "http_handler.h"

pthread_once_t ponce = PTHREAD_ONCE_INIT;

static const int KDEFAULT_INTFC_SIZE = 2048;
pthread_once_t ponce1 = PTHREAD_ONCE_INIT;
std::vector<http_intc_ptr> leak_prevent1;

bool kish::reg_http_interface(const http_intc_ptr &registrant) {
    pthread_once(&ponce1, []() -> void {
        leak_prevent1.reserve(KDEFAULT_INTFC_SIZE);
    });
    if (registrant) {
        try {
            registrant->on_reg_in(kish::http_handler::GLO_INTFC_HOLDER);
            leak_prevent1.emplace_back(registrant);
            return true;
        } catch (...) {

        }
    }
    return false;
}


bool kish::reg_http_resolver(const http_resol_ptr &registrant) {
    pthread_once(&ponce, []() -> void {
        kish::http_handler::GLO_RESOLR_MAPPERS.reserve(kish::http_handler::KDEFAULT_RESOLVER_SIZE);
    });
    try {
        kish::http_handler::GLO_RESOLR_MAPPERS.emplace_back(registrant);
        return true;
    } catch (const std::exception &e) {
        // todo: error will occur?
    }
    return false;
}
