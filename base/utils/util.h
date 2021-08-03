//
// Created by 蒋澳然 on 2021/8/3.
// email: 875691208@qq.com
// $desc
//

template<class T>
inline T coerce_in(const T &a, const T &b, const T &c) {
    if (a < b) {
        return b;
    }
    if (a > c) {
        return c;
    }
    return a;
}