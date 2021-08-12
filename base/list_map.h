//
// Created by 蒋澳然 on 2021/8/11.
// email: 875691208@qq.com
// $desc
//

#ifndef WEBKISH_LIST_MAP_H
#define WEBKISH_LIST_MAP_H

#include "base.h"
#include "vector"
#include "new"

struct not_found_exception : public std::exception {
    const char *what() const noexcept override {
        return "element not found!";
    }
};

namespace kish {

    // list_map存储的相同键时，是紧凑型排列的，同一个键可能有多个不同的值，即支持重复插入
    template<class key_t, class value_t, class container_t=std::vector<std::pair<key_t, value_t>>, bool case_sensitive = false>
    class list_map : copyable {
    public:
        using iter = typename container_t::iterator;
        using const_iter = typename container_t::const_iterator;
        using reference = value_t &;
        using const_reference = const value_t &;

        list_map() = default;

        list_map(const list_map &other)
                : container(other.container) {
        }

        list_map(const list_map &&other) noexcept
                : container(std::move(other.container)) {
        }

        list_map &operator=(const list_map &other) {
            list_map<key_t, value_t> tmp(other);
            swap(tmp);
            return *this;
        }

        list_map &operator=(list_map &&other) noexcept {
            container = std::move(other.container);
            return *this;
        }

        void swap(list_map &other) {
            container.swap(other.container);
        }

        const_iter begin() const {
            return container.begin();
        }

        const_iter end() const {
            return container.end();
        }

        // todo: 实现大小写敏感查找和插入
        const_iter find(const key_t &key) const {
            typename container_t::const_iterator it = container.begin();
            typename container_t::const_iterator it_end = container.end();
            while (it != it_end) {
                if (it->first == key) {
                    return it;
                }
                it++;
            }
            return it_end;
        }

        iter find(const key_t &key) {
            typename container_t::iterator it = container.begin();
            typename container_t::iterator it_end = container.end();
            while (it != it_end) {
                if (it->first == key) {
                    return it;
                }
                it++;
            }
            return it_end;
        }

        iter end() {
            return container.end();
        }

        reference at(const key_t &key) {
            iter it = find(key);
            if (it != container.end()) {
                return it->second;
            } else {
                throw not_found_exception();
            }
        }

        const_reference at(const key_t &key) const {
            const_iter it = find(key);
            if (it != container.end()) {
                return it->second;
            } else {
                throw not_found_exception();
            }
        }

        const_reference operator[](const key_t &key) const {
            const_iter it = find(key);
            if (it != container.end()) {
                return it->second;
            } else {
                throw not_found_exception();
            }
        }

        reference operator[](const key_t &key) {
            iter it = find(key);
            if (it != container.end()) {
                return it->second;
            } else {
                throw not_found_exception();
            }
        }

        iter insert(const std::pair<key_t, value_t> &ele) {
            // 将元素插入到相同元素组的最后一个
            iter it = find(ele.first);
            while (it != container.end() && it->first == ele.first) {
                it++;
            }
            return container.insert(it, ele);
        }

        void clear() {
            container.clear();
        }

    private:
        container_t container{};

    };

}


#endif //WEBKISH_LIST_MAP_H
