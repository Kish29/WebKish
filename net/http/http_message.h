//
// Created by 蒋澳然 on 2021/8/13.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_MESSAGE_H
#define WEBKISH_HTTP_MESSAGE_H

#include "base.h"
#include "list_map.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "multipart_parser.h"
#include "llhttp.h"
#ifdef __cplusplus
}
#endif

#include "map"
#include "http_transform.h"
#include "vector"
#include "kish_utils.h"


#define MIME_DEFAULT    "mime/default"

// T[text] text type
#define MIME_T_HTML     "text/html"
#define MIME_T_TXT      "text/plain"
#define MIME_T_XML      "text/xml"
#define MIME_T_C        "text/plain"
#define MIME_T_HTM      "text/html"
#define MIME_T_DFT      "text/plain"

// M[media] media type
#define MIME_M_AVI      "video/x-msvideo"
#define MIME_M_MP3      "audio/mp3"

#define MIME_M_BMP      "image/bmp"
#define MIME_M_GIF      "image/gif"
#define MIME_M_ICO      "image/x-icon"
#define MIME_M_JPG      "image/jpeg"
#define MIME_M_PNG      "image/png"

// A[application] application type
#define MIME_A_GZ       "application/x-gzip"
#define MIME_A_DOC      "application/msword"
#define MIME_A_URL      "application/x-www-form-urlencoded"
#define MIME_A_PDF      "application/pdf"
#define MIME_A_XML      "application/xml"
#define MIME_A_JSON     "application/json"
#define MIME_A_XHTML    "application/xhtml+xml"
#define MIME_A_BSTRM    "application/octet-stream"

// F[form]  form type
#define MIME_F_FORM     "multipart/form-data"

#define SPACE   ' '
#define COLON   ':'
#define CRLF    "\r\n"

#define CONNECTION_KEY      "Connection"
#define KEEP_ALIVE_VAL      "Keep-Alive"
#define CLOSE_VAL           "close"
#define CONTENT_TYPE_KEY    "Content-Type"
#define CONTENT_LENGTH_KEY  "Content-Length"
#define TRANSFER_TYPE_KEY   "Transfer-Encoding"

#define CHARSET_UTF8        "UTF-8"

namespace kish {

    using std::string;
    typedef list_map<string, http_transform> param_container;

    enum keep_alive_t {
        CLOSE_IMM,      // close immediate
        KEEP_ALIVE, // keep alive forever
        SET_TIMEOUT // set timeout
    };

    class http_message : public message_type {
    public:
        typedef std::map<std::string, std::string> header_item;

        uint8_t ver_major;
        uint8_t ver_minor;
        header_item headers{};
        std::vector<string> contents{};

        keep_alive_t alive;
        // 如果 alive 类型为 has_set_timeout，该字段会被设置
        uint32_t timeout{60};

        param_container params{};

        const http_transform &get_param(const string &key);

        virtual void parse_params_in_contents();

        void set_content_length(size_t len);

        size_t content_length() const;

        void set_content_type(const string &type);

        void set_content_type_with_charset(const string &type, const string &charset = CHARSET_UTF8);

        const string &get_content_type() const;

        void set_transfer_encoding(const string &enc);

        const string &get_transfer_encoding() const;

        http_message() : ver_major(1), ver_minor(0), alive(KEEP_ALIVE) {};

        static const std::string NO_CONTENT_TYPE;
        static const std::string NO_TRANSFER_ENCODE;
        static const std::string JSON_PARAM;
        static const std::string EMPTY_PARAM;
    };

}

const static std::map<int, std::string> RESP_STAT_CODE_MAP = {
// see https://developer.mozilla.org/en-US/docs/Web/HTTP/Status#client_error_responses
        {100, "Continue"},
        {101, "Switching Protocols"},
        {102, "Processing"},
        {103, "Early Hints"},
        {200, "OK"},
        {201, "Created"},
        {202, "Accepted"},
        {203, "Non-Authoritative Information"},
        {204, "No Content"},
        {205, "Reset Content"},
        {206, "Partial Content"},
        {300, "Multiple Choices"},
        {301, "Moved Permanently"},
        {302, "Found"},
        {303, "See Other"},
        {304, "Not Modified"},
        {305, "Use Proxy"},
        {307, "Temporary Redirect"},
        {308, "Permanent Redirect"},
        {400, "Bad Request"},
        {401, "Unauthorized"},
        {402, "Payment Required"},
        {403, "Forbidden"},
        {404, "Not Found"},
        {405, "Method Not Allowed"},
        {406, "Not Acceptable"},
        {407, "Proxy Authentication Required"},
        {408, "Request Timeout"},
        {409, "Conflict"},
        {410, "Gone"},
        {411, "Length Required"},
// ...
        {500, "Internal Server Error"},
        {501, "Not Implemented"},
        {502, "Bad Gateway"},
        {503, "Service Unavailable"},
        {504, "Gateway Timeout"},
        {505, "HTTP Version Not Supported"},
        {506, "Variant Also Negotiates"},
        {507, "Insufficient Storage"},
        {508, "Loop Detected"},
        {510, "Not Extended"},
        {511, "Network Authentication Required"}
};

#endif //WEBKISH_HTTP_MESSAGE_H
