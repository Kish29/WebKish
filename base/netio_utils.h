//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_NETIO_UTILS_H
#define WEBKISH_NETIO_UTILS_H

#include "endian.h"
#include "cstdint"

// 网络子节序都是大端法表示的
inline uint64_t host2network64(uint64_t host64) {
    return htobe64(host64);
}

inline uint32_t host2network32(uint32_t host32) {
    return htobe32(host32);
}

inline uint16_t host2network16(uint16_t host16) {
    return htobe16(host16);
}

inline uint64_t network2host64(uint64_t net64) {
    return be64toh(net64);
}

inline uint32_t network2host32(uint32_t net32) {
    return be32toh(net32);
}

inline uint16_t network2host16(uint16_t net16) {
    return be16toh(net16);
}


#endif //WEBKISH_NETIO_UTILS_H
