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

void test1(kish::http_request_parser &req_parser) {
    // -------- test get ---------
    const char *text = "GET /uri?arg1=test;arg1=%20%21;arg3=test HTTP/2.0\r\n"
                       "Connection: Keep-Alive\r\n"
                       "Content-Length: 0\r\n"
                       "\r\n";          // passed
    size_t len = strlen(text);
    req_parser.parse(text, len);

    req_parser.parse(text, len);
}

void test2(kish::http_request_parser &req_parser) {
    // ------- test post ---------
    const char *text = "POST /handler.cfm HTTP/1.1\n"
                       "Accept: text/*\n"
                       "Content-Type: multipart/form-data; boundary=----------Ij5ae0ae0KM7GI3KM7ei4cH2ei4gL6\n"
                       "User-Agent: Shockwave Flash\n"
                       "Host: www.example.com\n"
                       "Content-Length: 416\n"
                       "Connection: Keep-Alive\n"
                       "Cache-Control: no-cache\n"
                       "\n"
                       "------------Ij5GI3GI3ei4GI3ei4KM7GI3KM7KM7\n"
                       "Content-Disposition: form-data; name=\"Filename\"\n"
                       "\n"
                       "MyFile.jpg\n"
                       "------------Ij5GI3GI3ei4GI3ei4KM7GI3KM7KM7\n"
                       "Content-Disposition: form-data; name=\"photo\"; filename=\"MyFile.jpg\"\n"
                       "Content-Type: application/octet-stream\n"
                       "\n"
                       "FileDataHere\n"
                       "------------Ij5GI3GI3ei4GI3ei4KM7GI3KM7KM7\n"
                       "Content-Disposition: form-data; name=\"Upload\"\n"
                       "\n"
                       "Submit Query\n"
                       "\n"
                       "------------Ij5GI3GI3ei4GI3ei4KM7GI3KM7KM7--\n";     // passed


    size_t len = strlen(text);
    req_parser.parse(text, len);
}

void test3() {
    const char *text = "--WebAppBoundary\r\n"
                       "Content-Disposition: form-data; name=\"field-name\"\r\n"
                       "Content-Type: */*; charset=UTF-8\r\n"
                       "Content-Transfer-Encoding: 8bit\r\n"
                       "\r\n"
                       "field-value_t\r\n"
                       "--WebAppBoundary--\r\n";

    printf("%lu", strlen(text));
}

int main() {
    kish::http_request_parser req_parser;
    req_parser.set_on_parse_error(on_parse_error);
    req_parser.set_on_request_uri_parse_complete(on_req_uri_complete);
    req_parser.set_on_headers_parse_complete(on_req_headers_complete);
    req_parser.set_on_message_parser_complete(on_req_message_complete);

//    test1(req_parser);
    test2(req_parser);
//    test3();

    return 0;

}
