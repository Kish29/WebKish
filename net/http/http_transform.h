//
// Created by 蒋澳然 on 2021/8/11.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_HTTP_TRANSFORM_H
#define WEBKISH_HTTP_TRANSFORM_H

#include "base.h"
#include "vector"
#include "../../thirdparty/CJsonObject.hpp"

using namespace neb;

namespace kish {

    // transform类可以在多个类型中进行转换
    class http_transform : copyable {
    public:

        explicit operator const CJsonObject &() const;

        explicit operator CJsonObject();

        explicit operator int() const;

        explicit operator uint32_t() const;

        explicit operator int64_t() const;

        explicit operator uint64_t() const;

        explicit operator double() const;

        explicit operator bool() const;

        explicit operator const std::string &() const;

        explicit operator std::string();

    public:

        http_transform() = default;

        explicit http_transform(const char *o);

    protected:
        std::string stringval{};

        double doubleval{};
        bool boolval{};

        bool number_cast{false};
        bool bool_cast{false};

        CJsonObject jsonval{};
        bool jsonval_cast{false};
    };

}


#endif //WEBKISH_HTTP_TRANSFORM_H
