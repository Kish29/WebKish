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
#include "server_config.h"

namespace kish {

    class http_handler;

    using std::string;

    class http_interface;

    class http_interface_holder;

    class http_resolver;

    typedef std::function<void(const http_request_ptr &, http_response &)> hi_func;

    // http接口，表示能处理某一个uri请求
    class http_interface : copyable {
    public:
        virtual void on_reg_in(http_interface_holder &holder) = 0;
    };

    // resolver表示能处理某一些uri，比如文件resolver
    class http_resolver : copyable {
    public:

        // 表示该类是否能够处理该uri请求
        virtual bool can_resolve(const string &uri) = 0;

        virtual void on_request(const http_request_ptr &, http_response &response) = 0;

    };

    typedef std::shared_ptr<http_resolver> http_resol_ptr;
    typedef std::shared_ptr<http_interface> http_intc_ptr;

    // 注册接口
    bool reg_http_interface(const http_intc_ptr &);

    // 注册resolver类
    bool reg_http_resolver(const http_resol_ptr &);

    struct infc_type_t {
        hi_func infc;
        llhttp_method_t method;
    };

    class http_interface_holder : noncopyable {
    public:

        void regin(const string &uri, infc_type_t type) {
            hi_fcs.insert(std::make_pair(uri, std::move(type)));
        }

    private:
        typedef string uri;
        std::map<uri, infc_type_t> hi_fcs;

    private:
        friend class http_handler;

    };

}

#endif //WEBKISH_HTTP_INTERFACE_H
