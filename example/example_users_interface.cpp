//
// Created by 蒋澳然 on 2021/8/14.
// email: 875691208@qq.com
// $desc
//

#include "example_users_interface.h"

void example_users_interface::user_login(const http_request_ptr &request, http_response &response) {
    login_times++;
    response.update_stat(200);
    response.contents.emplace_back("登陆成功");
    response.set_content_type_with_charset(MIME_T_TXT);
}

void example_users_interface::user_regis(const http_request_ptr &request, http_response &response) {
    reg_times++;
    // 获取请求的内容类型
    const string &type = request->get_content_type();
    // 如果是json格式
    if (type == MIME_A_JSON) {
        // 注意！所有的强制转化，必须要进行异常catch，否则强转失败抛出异常导致本线程异常退出！
        try {
            // 将参数转化为json对象
            auto param_json = (CJsonObject) request->get_param(http_message::JSON_PARAM);
            string password;
            // 获取value
            param_json.Get("password", password);
            printf("password: %s\n", password.c_str());
            string username;
            param_json.Get("username", username);
            printf("username: %s\n", username.c_str());
            response.update_stat(200);
        } catch (const std::bad_cast &e) {
            response.update_stat(400);
        }
    } else {
        try {
            // 其他类型，获取想要的参数，转换即可
            string username = (string) request->get_param("username");
            string password = (string) request->get_param("password");
            printf("username: %s\n", username.c_str());
            printf("password: %s\n", password.c_str());
            response.update_stat(200);
        } catch (const std::bad_cast &e) {
            response.update_stat(400);
        }
    }
}

// on_reg_in方法会在reg_http_interface的时候调用，所以在这时候
// 注册我们要实现的方法，传入uri，接口，请求方法的类型
void example_users_interface::on_reg_in(http_interface_holder &holder) {
    holder.regin("/users/login", infc_type_t{
            .infc = std::bind(&example_users_interface::user_login, this, std::placeholders::_1, std::placeholders::_2),
            .method = HTTP_POST});
    holder.regin("/users/reg", infc_type_t{
            .infc = std::bind(&example_users_interface::user_regis, this, std::placeholders::_1, std::placeholders::_2),
            .method = HTTP_POST});
}

