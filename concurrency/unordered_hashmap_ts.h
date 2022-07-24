#ifndef UNORDERED_HASHMAP_TS_H
#define UNORDERED_HASHMAP_TS_H

#include <vector>
#include <memory>
#include <shared_mutex>  // shared_mutex (C++17)
#include "list_ts.h"

template<typename K, typename V, typename Hash=std::hash<K>>
class unordered_hashmap_ts {
    private:

        struct bucket {
            list_ts<std::pair<K,V>> list;
            // WARN: mutable keyword is neccessory. Otherwise, lock on the shared_mutex would cause EX_BAD_ACC
            mutable std::shared_mutex mutex;  
        };

        std::vector<bucket> buckets;

        // std::vector<list_ts<std::pair<K,V>>> buckets;
        // mutable std::vector<std::shared_mutex>  bkt_mutexes;
        Hash hasher;
    public:

        unordered_hashmap_ts();
        unordered_hashmap_ts(int bsize, const Hash &h=Hash());
        unordered_hashmap_ts(const unordered_hashmap_ts &) = delete;
        unordered_hashmap_ts &operator=(const unordered_hashmap_ts &) = delete;
        ~unordered_hashmap_ts();

        bool get(const K &key, V &val);
        void set(const K &key, const V &val);
        void del(const K &key);
        template<typename F>
        void for_each(F func);
};

template<typename K, typename V, typename Hash>
unordered_hashmap_ts<K, V, Hash>::unordered_hashmap_ts(): buckets(std::vector<list_ts<std::pair<K,V>>>(19)), hasher(Hash()) {}

// template<typename K, typename V, typename Hash>
// unordered_hashmap_ts<K, V, Hash>::unordered_hashmap_ts(int bsize, const Hash &h): buckets(std::vector<list_ts<std::pair<K,V>>>(bsize)), hasher(h) {}

template<typename K, typename V, typename Hash>
unordered_hashmap_ts<K, V, Hash>::unordered_hashmap_ts(int bsize, const Hash &h): buckets(std::vector<bucket>(bsize)), hasher(h) {}


template<typename K, typename V, typename Hash>
unordered_hashmap_ts<K, V, Hash>::~unordered_hashmap_ts() {}


template<typename K, typename V, typename Hash>
bool unordered_hashmap_ts<K, V, Hash>::get(const K &key, V &val) {

    std::pair<K,V> found_pair;
    bool found = buckets[hasher(key) % buckets.size()].list.find_first_if(
        [&](auto &pair) {
            return pair.first == key;
        },
        found_pair
    );

    if (found) {
        val = found_pair.second;
    }

    return found;
}

template<typename K, typename V, typename Hash>
void unordered_hashmap_ts<K, V, Hash>::set(const K &key, const V &new_val) {

    int bkt_idx = hasher(key) % buckets.size();
    std::unique_lock<std::shared_mutex> bkt_lock(buckets[bkt_idx].mutex);

    std::pair<K,V> new_pair(key, new_val);
    int updated = buckets[bkt_idx].list.update_if(
        [&](auto &pair) {
            return pair.first == key;
        },
        new_pair,
        true  //first_only
    );

    if (!updated) {
        buckets[bkt_idx].list.push_front(new_pair);
    }
}

template<typename K, typename V, typename Hash>
void unordered_hashmap_ts<K, V, Hash>::del(const K &key) {

    int bkt_idx = hasher(key) % buckets.size();
    buckets[bkt_idx].remove_if(
        [&](auto &pair) {
            return pair.first == key;
        }
    );
}

template<typename K, typename V, typename Hash>
template<typename F>
void unordered_hashmap_ts<K, V, Hash>::for_each(F func) {

    int num = buckets.size();
    for (int i = 0; i < num; ++i) {

        std::shared_lock<std::shared_mutex> bkt_lock(buckets[i].mutex);
        buckets[i].list.for_each(func);
    }
}

#endif