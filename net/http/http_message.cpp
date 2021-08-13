//
// Created by 蒋澳然 on 2021/8/13.
// email: 875691208@qq.com
// $desc
//

#include "http_message.h"
#include "logger.h"

const std::string kish::http_message::NO_CONTENT_TYPE = "NOCNT";
const std::string kish::http_message::NO_TRANSFER_ENCODE = "NOENC";
const std::string kish::http_message::JSON_PARAM = "JSON_PARAM";
const std::string kish::http_message::EMPTY_PARAM = "EMPTY_PARAM";

size_t kish::http_message::content_length() const {
    size_t length = 0;
    for (auto &c: contents) {
        length += c.size();
    }
    return length;
}

const kish::http_transform &kish::http_message::get_param(const std::string &key) {
    try {
        return params.at(key);
    } catch (const not_found_exception &e) {
        if (params.find(EMPTY_PARAM) == params.end()) {
            params.insert(std::make_pair(EMPTY_PARAM, http_transform()));
        }
    }
    return params.at(EMPTY_PARAM);
}

void kish::http_message::parse_params_in_contents() {
    // do nothing
}

void kish::http_message::set_content_length(size_t len) {
    headers.insert(std::make_pair(CONTENT_LENGTH_KEY, std::to_string(len)));
}

const std::string &kish::http_message::get_content_type() const {
    return headers.find(CONTENT_TYPE_KEY) != headers.end() ? headers.at(CONTENT_TYPE_KEY) : NO_CONTENT_TYPE;
}

void kish::http_message::set_transfer_encoding(const std::string &enc) {
    headers.insert(std::make_pair(TRANSFER_TYPE_KEY, enc));
}

const std::string &kish::http_message::get_transfer_encoding() const {
    return headers.find(TRANSFER_TYPE_KEY) != headers.end() ? headers.at(TRANSFER_TYPE_KEY) : NO_TRANSFER_ENCODE;
}

void kish::http_message::set_content_type(const string &type) {
    headers.insert(std::make_pair(CONTENT_TYPE_KEY, type));
}

void kish::http_message::set_content_type_with_charset(const string &type, const string &charset) {
    if (!charset.empty()) {
        string t = type + "; charset=" + kish_toupper(charset);
        headers.insert(std::make_pair(CONTENT_TYPE_KEY, std::move(t)));
    } else {
        headers.insert(std::make_pair(CONTENT_TYPE_KEY, type));
    }
}
