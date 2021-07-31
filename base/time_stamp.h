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

    class time_stamp : copyable, public printable {
    public:
        time_stamp();

        std::string tostring() const override;

        std::string get_format_time() const;

    private:
        // 从1970年过去的时间 毫秒
        int64_t mils_epoch;

    };
}


#endif //WEBKISH_TIME_STAMP_H
