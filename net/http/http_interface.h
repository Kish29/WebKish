//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_INTERFACE_H
#define WEBKISH_HTTP_INTERFACE_H

#include "http_parser.h"
#include "vector"

namespace kish {

    using std::string;

    class http_interface;

    typedef std::shared_ptr<http_interface> http_infc_ptr;

    // 注册接口
    bool reg_http_resolver(const http_infc_ptr &);

    // http请求接口，当一个url到来的时候，应当返回什么样的响应
    // 该类必须进行注册才能生效


    typedef std::map<string, string> parameters;
    typedef std::vector<string> param_list;

    struct infc_type {
        // 不包含参数的url
        string base_url{};
        req_method method{GET};
        // 需要的参数
        param_list needed_params{};

        infc_type(string url, req_method m) : base_url(std::move(url)), method(m) {}
    };

    class http_interface : copyable {
    public:
        // todo: url包含参数，所以  on_request 的入餐应当是一个结构体
        // todo: 序列化 RPC 参数

        virtual infc_type observe_url() const = 0;

        // on_request只返回 status code 和 body
        // todo: 支持RESTFUL风格
        virtual simp_resp on_request(const parameters &params) = 0;
    };

}

#endif //WEBKISH_HTTP_INTERFACE_H
