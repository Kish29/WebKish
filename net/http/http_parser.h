//
// Created by 蒋澳然 on 2021/8/1.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_PARSER_H
#define WEBKISH_HTTP_PARSER_H

#include "base.h"
#include "map"

namespace kish {

    using std::string;

    enum http_version {
        HTTP010,    // http/1.0
        HTTP011,    // http/1.1
        HTTP020     // http/2.0
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

    typedef std::map<string, string> head_items;

    // method + uri + version
    // headers
    // body
    struct request {
        req_method method;
        string uri;
        http_version version;
        head_items params;
        string body;

        request(
                req_method method,
                string uri,
                http_version version,
                head_items params,
                string body
        ) : method(method),
            uri(std::move(uri)),
            version(version),
            params(std::move(params)),
            body(std::move(body)) {}
    };

    // version + code + msg
    // headers
    // body
    struct simp_resp {
        int status{400};
        // todo: body可被序列化 RPC
        string body{};

        simp_resp(int st, string bd) : status(st), body(std::move(bd)) {}
    };

    struct response {
        http_version version;
        simp_resp smp;
        string msg;
        head_items params;
    };

    class http_parser : copyable {
    public:

    };

}


#endif //WEBKISH_HTTP_PARSER_H
