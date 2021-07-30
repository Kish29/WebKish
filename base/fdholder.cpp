//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#include "fdholder.h"

kish::fdholder::~fdholder() {
    // todo: delete this print
    printf("~fdholder() invoked\n");
}

bool kish::fdholder::closed() const {
    return false;
}

bool kish::fdholder::dead() const {
    return false;
};
