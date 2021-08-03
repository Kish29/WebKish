//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_INTERFACE_H
#define WEBKISH_HTTP_INTERFACE_H

#include "http_parser.h"
#include "vector"
#include "unordered_set"

namespace kish {

    using std::string;

    class http_interface;

    typedef std::shared_ptr<http_interface> http_infc_ptr;

    // 注册接口
    bool reg_http_interfc(const http_infc_ptr &);

    // http请求接口，当一个url到来的时候，应当返回什么样的响应
    // 该类必须进行注册才能生效


    typedef std::map<string, string> parameters;
    typedef std::vector<string> param_list;

    struct infc_type {
        // 不包含参数的url
        string base_url{};
        req_method method{GET};

        infc_type(string url, req_method m) : base_url(std::move(url)), method(m) {}
    };

    // resolver表示方法
    class http_interface : copyable {
    public:

        http_interface() {
            // 默认保留两个方法
            resolver_list.reserve(KDEFAULT_RESOLVER_SIZE);
        }

        // todo: url包含参数，所以  on_request 的入餐应当是一个结构体
        // todo: 序列化 RPC 参数
        virtual ~http_interface() = default;

        // 表示该类是否能够处理该uri请求
        virtual bool can_resolve(const string &uri) {
            return resolver_list.find(uri) != resolver_list.end();
        }

        // todo: 支持RESTFUL风格
        virtual simp_resp on_request(const string &uri, const parameters &params) {
            // 在这里判断uri是否符合
            return simp_resp{400, ""};
        }

    protected:
        std::unordered_set<string> resolver_list;

    private:
        static const int KDEFAULT_RESOLVER_SIZE = 2;

    };

}

#endif //WEBKISH_HTTP_INTERFACE_H
