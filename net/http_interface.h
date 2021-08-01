//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_INTERFACE_H
#define WEBKISH_HTTP_INTERFACE_H

#include <utility>

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
    struct lit_resp {
        int status{400};
        string body{};

        lit_resp(int st, string bd) : status(st), body(std::move(bd)) {}
    };

    enum req_method {
        GET,    // GET方法请求一个指定资源的表示形式. 使用GET的请求应该只被用于获取数据.
        HEAD,   // HEAD方法请求一个与GET请求的响应相同的响应，但没有响应体.
        POST,   // POST方法用于将实体提交到指定的资源，通常导致在服务器上的状态变化或副作用.
        PUT,    // PUT方法用请求有效载荷替换目标资源的所有当前表示。会调用数据库update操作
        DELETE, // DELETE方法删除指定的资源。
        CONNECT,    // CONNECT方法建立一个到由目标资源标识的服务器的隧道。
        OPTIONS,  // OPTIONS方法用于描述目标资源的通信选项。
        TRACE,  // TRACE方法沿着到目标资源的路径执行一个消息环回测试。
        PATCH   // PATCH方法用于对资源应用部分修改。
    };

    struct infc_type {
        // 不包含参数的url
        string base_url{};
        req_method method{GET};

        infc_type(string url, req_method m) : base_url(std::move(url)), method(m) {}
    };

    typedef std::map<string, string> request_param;

    class http_interface : copyable {
    public:
        // todo: url包含参数，所以  on_request 的入餐应当是一个结构体
        // todo: 序列化 RPC 参数

        virtual infc_type observe_url() const = 0;

        // on_request只返回 status code 和 body
        // todo: 支持RESTFUL风格
        virtual lit_resp on_request(const request_param &params) = 0;
    };

}

#endif //WEBKISH_HTTP_INTERFACE_H
