#include <vector>
#include <thread>
#include <iostream>
#include <cassert>

#include <functional>

#include "../unordered_hashmap_ts.h"

void set_int_seq(unordered_hashmap_ts<int, int> &hmap, int beg, int step, int end) {

    for (int i = beg; i < end; i += step) {
        hmap.set(i, -i);
    }
}

void del_int_seq(unordered_hashmap_ts<int, int> &hmap, int beg, int step, int end) {

    for (int i = beg; i < end; i += step) {
        hmap.del(i);
    }
}

void get_int_seq(unordered_hashmap_ts<int, int> &hmap, int beg, int step, int end) {

    bool found;
    int val;
    for (int i = beg; i < end; i += step) {
        found = hmap.get(i, val);
    }
}

int main(int argc, char **argv) {

    {
        std::cout << "Case 1: concurrent set()" << std::endl; 
        unordered_hashmap_ts<int, int> hashmap(73);

        int num_threads = 100;
        int num_nodes = 10000;
        std::vector<std::thread> threads(num_threads);
        for (int i = 0; i < num_threads; ++i) {
            threads[i] = std::thread(set_int_seq, std::ref(hashmap), 0, 1, num_nodes); 
        }

        for (int i = 0; i < threads.size(); ++i) {
            threads[i].join();
        }

        int count = 0;
        hashmap.for_each([&count](auto &pair) {
            count += 1;
        });
        std::cout << "Total: " << count << std::endl;
    }

    {
        std::cout << "Case 1: concurrent set() and del()" << std::endl; 

        unordered_hashmap_ts<int, int> hashmap(73);

        int num_set_threads = 100;
        int num_node_total = 10000;
        std::vector<std::thread> set_threads(num_set_threads);
        for (int i = 0; i < num_set_threads; ++i) {
            set_threads[i] = std::thread(set_int_seq, std::ref(hashmap), 0, 1, num_node_total); 
        }

        int num_del_threads = 50;
        std::vector<std::thread> del_threads(num_del_threads);
        for (int i = 0; i < num_del_threads; ++i) {
            del_threads[i] = std::thread(del_int_seq, std::ref(hashmap), 0, 1, num_node_total/2); 
        }

        for (int i = 0; i < set_threads.size(); ++i) {
            set_threads[i].join();
        }
        for (int i = 0; i < del_threads.size(); ++i) {
            del_threads[i].join();
        }

        int count = 0;
        hashmap.for_each([&count](auto &pair) {
            count += 1;
        });
        std::cout << "Total (random): " << count << std::endl;
    }

    {
        std::cout << "Case 1: concurrent set() and get()" << std::endl; 

        unordered_hashmap_ts<int, int> hashmap(73);

        int num_set_threads = 100;
        int num_node_total = 10000;
        std::vector<std::thread> set_threads(num_set_threads);
        for (int i = 0; i < num_set_threads; ++i) {
            set_threads[i] = std::thread(set_int_seq, std::ref(hashmap), 0, 1, num_node_total); 
        }

        int num_get_threads = 50;
        std::vector<std::thread> get_threads(num_get_threads);
        for (int i = 0; i < num_get_threads; ++i) {
            get_threads[i] = std::thread(get_int_seq, std::ref(hashmap), 0, 1, num_node_total); 
        }

        for (int i = 0; i < set_threads.size(); ++i) {
            set_threads[i].join();
        }
        for (int i = 0; i < get_threads.size(); ++i) {
            get_threads[i].join();
        }

        int count = 0;
        hashmap.for_each([&count](auto &pair) {
            count += 1;
        });
        std::cout << "Total: " << count << std::endl;
    }
}