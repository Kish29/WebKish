//
// Created by 蒋澳然 on 2021/7/31.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_TIME_STAMP_H
#define WEBKISH_TIME_STAMP_H

#include "base.h"
#include "string"

namespace kish {

    const int KMILS_PER_SEC = 1000;
    const int KMICRO_PER_SEC = 1000 * KMILS_PER_SEC;
    const int KNANO_PER_SEC = 1000 * KMICRO_PER_SEC;

    class time_stamp : copyable, public printable {
    public:
        // 默认取当时的机器时间
        time_stamp();

        std::string tostring() const override;

        // 是否使用UTC时间，否则使用local时间
        std::string get_format_time(bool show_micro = false, bool UTC = false) const;

        uint64_t get_time_milsec() const;

        uint64_t get_time_sec() const;

    private:
        // 从1900年过去的时间
        timeval tm_epoch{};
    };
}


#endif //WEBKISH_TIME_STAMP_H
