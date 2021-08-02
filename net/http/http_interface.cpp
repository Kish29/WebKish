//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#include "http_interface.h"
#include "http_handler.h"

bool kish::reg_http_resolver(const http_infc_ptr &registrant) {
#define GET_ENTRY http_handler::GLO_GET_MAPPERS
#define HEAD_ENTRY http_handler::GLO_HEAD_MAPPERS
#define POST_ENTRY http_handler::GLO_POST_MAPPERS
#define PUT_ENTRY http_handler::GLO_PUT_MAPPERS
#define DELETE_ENTRY http_handler::GLO_DELETE_MAPPERS
#define CONNECT_ENTRY http_handler::GLO_CONNECT_MAPPERS
#define OPTIONS_ENTRY http_handler::GLO_OPTIONS_MAPPERS
#define TRACE_ENTRY http_handler::GLO_TRACE_MAPPERS
#define PATCH_ENTRY http_handler::GLO_PATCH_MAPPERS
    if (registrant) {
        const string url = registrant->observe_url().base_url;
        const req_method method = registrant->observe_url().method;
        try {
            switch (method) {
                case GET:
                    GET_ENTRY.insert(std::make_pair(url, registrant));
                    break;
                case HEAD:
                    HEAD_ENTRY.insert(std::make_pair(url, registrant));
                    break;
                case POST:
                    POST_ENTRY.insert(std::make_pair(url, registrant));
                    break;
                case PUT:
                    PUT_ENTRY.insert(std::make_pair(url, registrant));
                    break;
                case DELETE:
                    DELETE_ENTRY.insert(std::make_pair(url, registrant));
                    break;
                case CONNECT:
                    CONNECT_ENTRY.insert(std::make_pair(url, registrant));
                    break;
                case OPTIONS:
                    OPTIONS_ENTRY.insert(std::make_pair(url, registrant));
                    break;
                case TRACE:
                    TRACE_ENTRY.insert(std::make_pair(url, registrant));
                    break;
                case PATCH:
                    PATCH_ENTRY.insert(std::make_pair(url, registrant));
                    break;
            }
        } catch (const std::exception &e) {
            // todo:
        }
        return true;
    }
    return false;
}
