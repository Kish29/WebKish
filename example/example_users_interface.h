//
// Created by 蒋澳然 on 2021/8/14.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_EXAMPLE_USERS_INTERFACE_H
#define WEBKISH_EXAMPLE_USERS_INTERFACE_H

#include "http_interface.h"

using namespace kish;

class example_users_interface : public kish::http_interface {
public:

    void on_reg_in(http_interface_holder &holder) override;

    void user_login(const http_request_ptr &request, http_response &response);

    void user_regis(const http_request_ptr &request, http_response &response);

private:
    size_t login_times{0};
    size_t reg_times{0};
};


#endif //WEBKISH_EXAMPLE_USERS_INTERFACE_H
