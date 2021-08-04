//
// Created by 蒋澳然 on 2021/8/5.
// email: 875691208@qq.com
// $desc
//

#include "http_parser.h"
#include "cstring"

void show_res(const char *text) {
    kish::http_parser parser(kish::http_parser::REQUEST);
    if (parser.parse(text, strlen(text))) {
        auto p = parser.get_req();
        if (p) {
            printf("%s", p->tomessage().c_str());
        }
    }
}

void test_post() {
    const char text[] = "POST /uri.cgi HTTP/1.1\r\n"
                        "From: user@example.com\r\n"
                        "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:18.0) Gecko/20100101 Firefox/18.0\r\n"
                        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                        "Accept-Language: en-US,en;q=0.5\r\n"
                        "Accept-Encoding: gzip, deflate\r\n"
                        "Content-Type: text/plain\r\n"
                        "Transfer-Encoding: chunked\r\n"
                        "Host: 127.0.0.1\r\n"
                        "\r\n"
                        "23\r\n"
                        "This is the data in the first chunk\r\n"
                        "1A\r\n"
                        "and this is the second one\r\n"
                        "3\r\n"
                        "con\r\n"
                        "9\r\n"
                        "sequence\0\r\n"
                        "0\r\n\r\n";
    show_res(text);
}

void test_get() {
    const char *text = "GET /uri?arg1=test;arg1=%20%21;arg3=test\r\n";
    show_res(text);
}

int main() {
    test_post();
    test_get();
}
