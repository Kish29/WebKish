//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_INTERFACE_H
#define WEBKISH_HTTP_INTERFACE_H

#include "base.h"
#include "map"

namespace kish {

    using std::string;

    class http_interface;

    typedef std::shared_ptr<http_interface> http_infc_ptr;

    // 注册接口
    bool reg_http_resolver(const http_infc_ptr &);

    // http请求接口，当一个url到来的时候，应当返回什么样的响应
    // 该类必须进行注册才能生效

    // todo: 序列化 RPC
    /*struct request_mapper {
        // base_url是'？'前的路径
        string base_url{};

        typedef string key;
        typedef string value;
        std::map<key, value> params{};
    };*/
    typedef std::map<string, string> request_param;

    class http_interface : copyable {
    public:
        // todo: url包含参数，所以  on_request 的入餐应当是一个结构体
        // todo: 序列化 RPC 参数

        // on_request只返回一个body
        virtual string on_request(const request_param &params) = 0;

        virtual string observe_url() const = 0;
    };

}

#endif //WEBKISH_HTTP_INTERFACE_H
