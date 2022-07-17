#ifndef UNORDERED_HASHMAP_TS_H
#define UNORDERED_HASHMAP_TS_H

#include <vector>
#include <memory>
#include <mutex>
#include "list_ts.h"

template<typename K, typename V, typename Hash=std::hash<K>>
class unordered_hashmap_ts {
    private:

        std::vector<list_ts<std::pair<K,V>>> buckets;
        Hash hasher;
    public:

        unordered_hashmap_ts();
        unordered_hashmap_ts(int bsize, const Hash &h);
        unordered_hashmap_ts(const unordered_hashmap_ts &) = delete;
        unordered_hashmap_ts &operator=(const unordered_hashmap_ts &) = delete;
        ~unordered_hashmap_ts();

        bool get(const K &key, V &val);
        void set(const K &key, const V &val, bool add=true);
        void del(const K &key);
};

template<typename K, typename V, typename Hash>
unordered_hashmap_ts<K, V, Hash>::unordered_hashmap_ts(): buckets(std::vector<list_ts<std::pair<K,V>>>(19)), hasher(Hash()) {}

template<typename K, typename V, typename Hash>
unordered_hashmap_ts<K, V, Hash>::unordered_hashmap_ts(int bsize, const Hash &h): buckets(std::vector<list_ts<std::pair<K,V>>>(bsize)), hasher(h) {}

template<typename K, typename V, typename Hash>
unordered_hashmap_ts<K, V, Hash>::~unordered_hashmap_ts() {}


template<typename K, typename V, typename Hash>
bool unordered_hashmap_ts<K, V, Hash>::get(const K &key, V &val) {

    std::pair<K,V> found_pair;
    bool found = buckets[hasher(key) % buckets.size()].find_first_if(
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
void unordered_hashmap_ts<K, V, Hash>::set(const K &key, const V &new_val, bool add) {

    int bucket_idx = hasher(key) % buckets.size();

    std::pair<K,V> new_pair(key, new_val);
    buckets[bucket_idx].update_first_if(
        [&](auto &pair) {
            return pair.first == key;
        },
        new_pair,
        add
    );
}

template<typename K, typename V, typename Hash>
void unordered_hashmap_ts<K, V, Hash>::del(const K &key) {

    int bucket_idx = hasher(key) % buckets.size();
    buckets[bucket_idx].remove_if(
        [&](auto &pair) {
            return pair.first == key;
        }
    );
}

#endif