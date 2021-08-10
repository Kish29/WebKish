//
// Created by 蒋澳然 on 2021/8/11.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_TRANSFORM_H
#define WEBKISH_HTTP_TRANSFORM_H

#include "base.h"
#include "vector"
#include "cJSON.h"

namespace kish {

#define cJSON_Number_Integer    (cJSON_Number | (1 << 8))
#define cJSON_Number_Double     (cJSON_Number | (1 << 9))

    class http_transform : copyable, public jsonable {
    public:

        explicit operator const std::vector<http_transform> &() const;

        explicit operator int() const;

        explicit operator const std::vector<int> &() const;

        explicit operator char() const;

        explicit operator const char *() const;

        explicit operator double() const;

        explicit operator const std::vector<double> &() const;

        explicit operator bool() const;

        explicit operator const std::string &() const;

        explicit operator const std::vector<std::string> &() const;

        const std::string &name() const { return m_name; }

        http_transform &set_name(const std::string &name) {
            m_name = name;
            return *this;
        }

        std::string tojson() const override;

    public:
        typedef http_transform self;

        http_transform() = default;

        explicit http_transform(const std::string &o) {
            stringval = o;
        }

        explicit http_transform(char *o) {
            stringval = std::string(o);
        }

    protected:
        std::string m_name;

        int type{cJSON_Raw};

        std::vector<http_transform> child;
        std::string stringval{};
        std::vector<string> strings;
        int intval{};
        std::vector<int> integers;
        // trick names here :-)
        double doubleval{};
        std::vector<double> doubles;

    };

}


#endif //WEBKISH_HTTP_TRANSFORM_H
