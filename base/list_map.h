//
// Created by 蒋澳然 on 2021/8/11.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_LIST_MAP_H
#define WEBKISH_LIST_MAP_H

#include "base.h"
#include "vector"

namespace kish {

    template<class key_t, class value_t, class container_t=std::vector<std::pair<key_t, value_t>>, bool case_sensitive = false>
    class list_map : copyable {
    public:
        using iter = typename container_t::iterator;

        list_map(const list_map &);

        list_map &operator=(const ist_map &);

        iter find(const key_t &key) const {
            typename container_t::const_iterator it = container.begin();
            typename container_t::const_iterator it_end = container.end();
            while (it != it_end) {
                if (it->first == key) {

                }
            }
        }

    private:
        container_t container;

    };

}


#endif //WEBKISH_LIST_MAP_H
