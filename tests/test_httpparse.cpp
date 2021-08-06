//
// Created by 蒋澳然 on 2021/8/5.
// email: 875691208@qq.com
// $desc
//

#include "http_parser.h"
#include "cstring"

void on_parse_error() {
    printf("parse error!\n");
}

void on_req_uri_complete(const kish::http_request_ptr &request) {
    printf("on_req_uri_complete: %s\n", request->tomessage().c_str());
}

kish::parse_control on_req_headers_complete(const kish::http_request_ptr &request) {
    printf("on_req_headers_complete: %s\n", request->tomessage().c_str());
    return kish::GO_ON;
}

void on_req_message_complete(const kish::http_request_ptr &request) {
    printf("on_req_message_complete: %s\n", request->tomessage().c_str());
}

int main() {
    kish::http_request_parser req_parser;
    req_parser.set_on_parse_error(on_parse_error);
    req_parser.set_on_request_uri_parse_complete(on_req_uri_complete);
    req_parser.set_on_headers_parse_complete(on_req_headers_complete);
    req_parser.set_on_message_parser_complete(on_req_message_complete);

    // -------- test get ---------
    const char *text = "GET /uri?arg1=test;arg1=%20%21;arg3=test HTTP/2.0\r\n"
                       "Connection: Keep-Alive\r\n"
                       "Content-Length: 0\r\n"
                       "\r\n";          // passed
    size_t len = strlen(text);
    req_parser.parse(text, len);

    req_parser.parse(text, len);


    // ------- test post ---------
    text = "POST /uri.cgi HTTP/1.1\r\n"
           "From: user@example.com\r\n"
           "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:18.0) Gecko/20100101 Firefox/18.0\r\n"
           "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
           "Accept-Language: en-US,en;q=0.5\r\n"
           "Accept-Encoding: gzip, deflate\r\n"
           "Content-Type: text/html\r\n"
           "Transfer-Encoding: chunked\r\n"
           "Host: 127.0.0.1\r\n"
           "\r\n"
           "23\r\n"
           "username=jiangaoran;password=123456\r\n"
           "1A\r\n"
           "and this is the second one\r\n"
           "3\r\n"
           "con\r\n"
           "8\r\n"
           "sequence\r\n"
           "0\r\n\r\n";         // passed

    len = strlen(text);
    req_parser.parse(text, len);


}
