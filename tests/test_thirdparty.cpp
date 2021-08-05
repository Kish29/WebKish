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
        printf("http ver is HTTP/%u.%u\n", t->http_major, t->http_minor);
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
    llhttp_init(&parser, HTTP_REQUEST, &settings);

/* Parse request! */
    const char *text = "GET / HTTP/1.0\r\n\r\n";
    int request_len = strlen(text);

    enum llhttp_errno err = llhttp_execute(&parser, text, request_len);
    if (llhttp_message_needs_eof(&parser)) {
        printf("needs eof!");
    }
    if (err == HPE_OK) {
        printf("Successfully parsed!\n");
    } else {
        fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err),
                parser.reason);
    }
}
