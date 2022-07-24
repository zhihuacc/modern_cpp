#include <vector>
#include <thread>
#include <iostream>
#include <cassert>

#include <functional>

#include "../list_ts.h"


//WARN: can not pass list_ts in the function by value or by reference,
//  because the constructor of thread will copy its arguments ANYWAY, but list_ts is not allowed to be copied.
//  Need use std::ref to pass list_ts to thread constructor.

void push_int_seq(list_ts<int> &l0, int beg, int step, int end) {

    for (int i = beg; i < end; i += step) {
        l0.push_front(i);
    }
}

void remove_int_seq(list_ts<int> &l0, int div, int r) {
    
    l0.remove_if([div, r](auto j) {return j % div == r;});
}

void update_int_seq(list_ts<int> &l0, int new_data, int div, int r) {
    
    l0.update_if([div, r](auto j) {return j % div == r;}, new_data);
}

int main(int argc, char **argv) {

    
    {
        //Case 1: concurrent push_front()
        std::cout << "Case 1: concurrent push_front()" << std::endl;

        list_ts<int> l0;
        int num_threads = 200;
        int num_nodes = 20000;
        std::vector<std::thread> threads(num_threads);
        for (int i = 0; i < num_threads; ++i) {
            threads[i] = std::thread(push_int_seq, std::ref(l0), i*num_nodes, 1, (i+1)*num_nodes); 
        }

        // std::chrono::milliseconds dura(2000);
        // std::this_thread::sleep_for(dura);
        for (int i = 0; i < threads.size(); ++i) {
            threads[i].join();
        }

        int count = 0;
        l0.for_each([&count](auto i) { count += 1;});
        std::cout << "Total " << count << std::endl;
        assert((count == num_nodes*num_threads));
    }

    {
        // Case 2: concurrent remove_if()
        std::cout << "Case 2: concurrent remove_if()" << std::endl;

        list_ts<int> l0;
        int total = 4000000;
        int count = 0;

        push_int_seq<int>(std::ref(l0), 0, 1, total);
        l0.for_each([&count](auto i) { count += 1;});
        std::cout << "Total at the start: " << count << std::endl;

        int num_threads = 200;
        std::vector<std::thread> threads(num_threads);
        for (int i = 0; i < num_threads; ++i) {
            threads[i] = std::thread(remove_int_seq, std::ref(l0), num_threads, i); 
        }

        // std::chrono::milliseconds dura(2000);
        // std::this_thread::sleep_for(dura);
        for (int i = 0; i < threads.size(); ++i) {
            threads[i].join();
        }

        count = 0;
        l0.for_each([&count](auto i) { count += 1;});
        std::cout << "Total at the end: " << count << std::endl;
        assert((count == 0));
    }

    {
        // Case 3: concurrent push_front() and remove_if()
        std::cout << "Case 3: concurrent push_front() and remove_if()" << std::endl;

        list_ts<int> l0;
        int num_nodes = 20000;
        int num_threads = 200;
        int total = num_nodes*num_threads;
        std::vector<std::thread> push_threads(num_threads);
        for (int i = 0; i < num_threads; ++i) {
            push_threads[i] = std::thread(push_int_se, std::ref(l0), 0, 1, num_nodes); 
        }

        std::chrono::milliseconds dura(1000);
        std::this_thread::sleep_for(dura);

        num_threads = 100;
        std::vector<std::thread> remove_threads(num_threads);
        for (int i = 0; i < num_threads; ++i) {
            remove_threads[i] = std::thread(remove_int_seq, std::ref(l0), num_threads, i); 
        }

        for (int i = 0; i < push_threads.size(); ++i) {
            push_threads[i].join();
        }
        for (int i = 0; i < remove_threads.size(); ++i) {
            remove_threads[i].join();
        }

        int count = 0;
        l0.for_each([&count](auto i) { count += 1;});
        std::cout << "Total at the end (random): " << count << std::endl;
    }

    {
        std::cout << "Case 4: concurrent remove_if() and update_if()" << std::endl;

        list_ts<int> l0;
        int total = 4000000;
        int count = 0;

        push_int_seq<int>(std::ref(l0), 0, 1, total);
        l0.for_each([&count](auto i) { count += 1;});
        std::cout << "Total at the start: " << count << std::endl;


        int num_threads = 100;
        std::vector<std::thread> remove_threads(num_threads);
        std::vector<std::thread> update_threads(num_threads);
        for (int i = 0; i < num_threads; ++i) {
            update_threads[i] = std::thread(update_int_seq, std::ref(l0), 0, num_threads, i); 
            remove_threads[i] = std::thread(remove_int_seq, std::ref(l0), num_threads, i+1); 
        }

        for (int i = 0; i < remove_threads.size(); ++i) {
            remove_threads[i].join();
        }
        for (int i = 0; i < update_threads.size(); ++i) {
            update_threads[i].join();
        }

        count = 0;
        l0.for_each([&count](auto i) { if (i==0)count += 1;});
        std::cout << "Total 0 at the end (random): " << count << std::endl;

        count = 0;
        l0.for_each([&count](auto i) {count += 1;});
        std::cout << "Total at the end (random): " << count << std::endl;
    }
}