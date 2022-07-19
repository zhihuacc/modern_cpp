#include <vector>
#include <thread>
#include <iostream>
#include <cassert>

#include <functional>

#include "../list_ts.h"

// list_ts<int> l0;
//WARN: can not pass list_ts in the function by value or by reference,
//  because the constructor of thread will copy its arguments ANYWAY, but list_ts is not allowed to be copied.
//  Need use std::ref to pass list_ts to thread constructor.
template<typename T>
void push_int_seq(list_ts<T> &l0, int beg, int step, int end) {

    for (int i = beg; i < end; i += step) {
        l0.push_front(i);
    }
}

template<typename T>
void remove_int_seq(list_ts<T> &l0, int beg, int end, int div, int r) {
    
    l0.remove_if([div, r](auto j) {return j % div == r;});
}

int main(int argc, char **argv) {

    
    {
        //Case 1: concurrent push_front()
        std::cout << "Case 1:" << std::endl;
        
        list_ts<int> l0;
        int num_threads = 200;
        int num_nodes = 20000;
        std::vector<std::thread> threads(num_threads);
        for (int i = 0; i < num_threads; ++i) {
            threads[i] = std::thread(push_int_seq<int>, std::ref(l0), i*num_nodes, 1, (i+1)*num_nodes); 
        }

        std::chrono::milliseconds dura(2000);
        std::this_thread::sleep_for(dura);
        for (int i = 0; i < num_threads; ++i) {
            threads[i].join();
        }

        int count = 0;
        l0.for_each([&count](auto i) { count += 1;});
        std::cout << "Total " << count << std::endl;
        assert((count == num_nodes*num_threads));
    }

    {
        // Case 2: concurrent remove_if()
        std::cout << "Case 2:" << std::endl;

        list_ts<int> l0;
        int num_threads = 200;
        int num_nodes = 20000;
        int total = num_nodes * num_threads;
        int count = 0;

        push_int_seq<int>(std::ref(l0), 0, 1, total);
        count = 0;
        l0.for_each([&count](auto i) { count += 1;});
        std::cout << "Total before removing: " << count << std::endl;

        num_threads = 200;
        std::vector<std::thread> threads(num_threads);
        for (int i = 0; i < num_threads; ++i) {
            threads[i] = std::thread(remove_int_seq<int>, std::ref(l0), 0, count, num_threads, i); 
        }

        std::chrono::milliseconds dura(2000);
        std::this_thread::sleep_for(dura);
        for (int i = 0; i < num_threads; ++i) {
            threads[i].join();
        }

        count = 0;
        l0.for_each([&count](auto i) { count += 1;});
        
        std::cout << "Total after removing: " << count << std::endl;
        assert((count == 0));
    }

    // // Case 3: concurrent push_front() and remove_if()
    // num_threads = 200;
    // threads = std::vector<std::thread>(num_threads);
    // for (int i = 0; i < num_threads; ++i) {
    //     threads[i] = std::thread(remove_int_seq, 0, total, num_threads, i); 
    // }
}