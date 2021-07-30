//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_FDHOLDER_H
#define WEBKISH_FDHOLDER_H


#include "unistd.h"
#include "event_handler.h"
#include "cstdio"

namespace kish {

    // 核心接口，fdholer接口表示该类持有一个文件描述符，在其生命周期停止时
    // 释放该文件描述符
    class fdholder {
    public:
        // 纯虚析构函数仍然需要实现，因为基类的析构函数始终会被调用
        virtual ~fdholder() = 0;

        virtual int fd() const = 0;

        virtual bool dead() const;

    };

}


#endif //WEBKISH_FDHOLDER_H
