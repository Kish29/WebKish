//
// Created by 蒋澳然 on 2021/8/4.
// email: 875691208@qq.com
// $desc
//

#include "llhttp.h"
#include "cstring"
#include "cstdio"

int on_message_start(llhttp_t *parser) {
    printf("message parse start!\n");
    return 0;
}

int on_url(llhttp_t *parser, const char *at, size_t len) {
    printf("method is %s\n", llhttp_method_name(static_cast<llhttp_method_t>(parser->method)));
    char *uri = new char[len + 1];
    memcpy(uri, at, len);
    *(uri + len) = '\0';
    printf("uri is %s, length is %lu\n", uri, len);
    delete[] uri;
    return 0;
}

int on_status(llhttp_t *parser, const char *at, size_t len) {
    char *status_code = new char[len + 1];
    memcpy(status_code, at, len);
    *(status_code + len) = '\0';
    printf("status_code is %s, length is %lu\n", status_code, len);
    delete[] status_code;
    return 0;
}

int on_header_filed(llhttp_t *parser, const char *at, size_t len) {
    return 0;
}

int on_header_value(llhttp_t *parser, const char *at, size_t len) {
    printf("HTTP/%u.%u\n", parser->http_major, parser->http_minor);
    return 0;
}

int on_headers_complete(llhttp_t *parser) {
    printf("on headers complete!\n");
    return 0;
}

int on_body(llhttp_t *parser, const char *at, size_t len) {
    char *body = new char[len + 1];
    memcpy(body, at, len);
    *(body + len) = '\0';
    printf("body is %s, length is %lu\n", body, len);
    delete[] body;
    return 0;
}


int on_message_complete(llhttp_t *parser) {
    printf("on message complete\n");
    return 0;
}

int on_chunk_header(llhttp_t *parser) {
    printf("chunk header length is %lu\n", parser->content_length);
    return 0;
}

int on_chunk_complete(llhttp_t *parser) {
    printf("on chunk complete\n");
    return 0;
}

int on_url_complete(llhttp_t *parser) {
    printf("on_url_complete\n");
    return 0;
}

int on_status_complete(llhttp_t *parser) {
    printf("on_status_complete\n");
    return 0;
}

int main() {
    llhttp_t parser;
    llhttp_settings_t settings;

/* Initialize user callbacks and settings */
    llhttp_settings_init(&settings);
    settings.on_message_begin = on_message_start;
    settings.on_url = on_url;
    settings.on_status = on_status;
    settings.on_header_field = on_header_filed;
    settings.on_header_value = on_header_value;
    settings.on_headers_complete = on_headers_complete;
    settings.on_body = on_body;
    settings.on_message_complete = on_message_complete;
    settings.on_chunk_header = on_chunk_header;
    settings.on_chunk_complete = on_chunk_complete;
    llhttp_init(&parser, HTTP_BOTH, &settings);

    const char *text = "PUT /url HTTP/1.1\r\n"
                       "Transfer-Encoding: chunked\r\n"
                       "\r\n"
                       "a\r\n"
                       "0123456789\r\n"
                       "0\r\n\r\n";

    size_t request_len = strlen(text);

    enum llhttp_errno err = llhttp_execute(&parser, text, request_len);

    if (err == HPE_OK) {
        printf("Successfully parsed!\n");
    } else {
        fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err),
                parser.reason);
    }

}
