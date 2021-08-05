//
// Created by 蒋澳然 on 2021/7/31.
// email: 875691208@qq.com
// $desc
//

#include "time_stamp.h"
#include "sys/time.h"

kish::time_stamp::time_stamp() {
    gettimeofday(&tm_epoch, nullptr);
}

std::string kish::time_stamp::get_format_time(bool show_micro, bool UTC) const {
    char buf[64]{};
    struct tm tmzone{};
    time_t secs = tm_epoch.tv_sec;
    if (UTC) {
        gmtime_r(&secs, &tmzone);
    } else {
        localtime_r(&secs, &tmzone);
    }
    if (show_micro) {
        // %02d不足两位，补0
        snprintf(
                buf,
                sizeof buf,
                "%4d-%02d-%02d %02d:%02d:%02d.%06ld",
                tmzone.tm_year + 1900,   /* Year	- 1900.  */
                tmzone.tm_mon + 1,       /* Month.	[0-11] */
                tmzone.tm_mday,          /* Day.		[1-31] */
                tmzone.tm_hour,          /* Hours.	[0-23] */
                tmzone.tm_min,           /* Minutes.	[0-59] */
                tmzone.tm_sec,           /* Seconds.	[0-60] (1 leap second) */
                tm_epoch.tv_usec
        );
    } else {
        snprintf(
                buf,
                sizeof buf,
                "%4d-%02d-%02d %02d:%02d:%02d",
                tmzone.tm_year + 1900,
                tmzone.tm_mon + 1,
                tmzone.tm_mday,
                tmzone.tm_hour,
                tmzone.tm_min,
                tmzone.tm_sec
        );
    }
    return buf;
}

std::string kish::time_stamp::tostring() const {
    char buf[32]{};
    // 保留6位，因为毫秒占3位，后3位才是微妙
    snprintf(buf, sizeof buf, "%ld.%06ld", tm_epoch.tv_sec, tm_epoch.tv_usec);
    return buf;
}

uint64_t kish::time_stamp::get_time_milsec() const {
    return tm_epoch.tv_sec * KMILS_PER_SEC + tm_epoch.tv_usec / (KMICRO_PER_SEC / KMILS_PER_SEC);
}

uint64_t kish::time_stamp::get_time_sec() const {
    return tm_epoch.tv_sec;
}
