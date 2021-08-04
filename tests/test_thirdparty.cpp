//
// Created by 蒋澳然 on 2021/8/4.
// email: 875691208@qq.com
// $desc
//

#include "llhttp.h"
#include "cstring"
#include "cstdio"

int handle_on_message_complete(llhttp_t *t) {
    if (t) {
        printf("method is %s\n", llhttp_method_name(static_cast<llhttp_method_t>(t->method)));
        printf("http ver is HTTP/%u.%u", t->http_major, t->http_minor);
    }
    return 0;
}

int main() {
    llhttp_t parser;
    llhttp_settings_t settings;

/* Initialize user callbacks and settings */
    llhttp_settings_init(&settings);

/* Set user callback */
    settings.on_message_complete = handle_on_message_complete;
    settings.on_url = [](llhttp_t *tt, const char *at, size_t length) -> int {
        printf("data is %s\n", (char *) tt->data);
        char *uri = new char[length + 1];
        memcpy(uri, at, length);
        *(uri + length) = '\0';
        printf("uri is %s, length is %lu\n", uri, length);
        delete[] uri;
        return 0;
    };
    settings.on_body = [](llhttp_t *tt, const char *at, size_t length) -> int {
        char body[length + 1];
        memcpy(body, at, length);
        *(body + length) = '\0';
        printf("contents is %s\n", body);
        return 0;
    };
    settings.on_header_field = [](llhttp_t *tt, const char *at, size_t length) -> int {
        char field[length + 1];
        memcpy(field, at, length);
        *(field + length) = '\0';
        printf("field is %s\n", field);
        return 0;
    };
    settings.on_header_value = [](llhttp_t *tt, const char *at, size_t length) -> int {
        char value[length + 1];
        memcpy(value, at, length);
        *(value + length) = '\0';
        printf("value is %s\n", value);
        return 0;
    };

/* Initialize the psr in HTTP_BOTH mode, meaning that it will select between
 * HTTP_REQUEST and HTTP_RESPONSE parsing automatically while reading the first
 * input.
 */
    llhttp_init(&parser, HTTP_BOTH, &settings);

/* Parse request! */
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
    int request_len = strlen(text);

    enum llhttp_errno err = llhttp_execute(&parser, text, request_len);
    if (err == HPE_OK) {
        /* Successfully parsed! */
    } else {
        fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err),
                parser.reason);
    }
}
