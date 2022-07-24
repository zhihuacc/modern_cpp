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
}