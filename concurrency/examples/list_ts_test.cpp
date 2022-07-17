#include <vector>
#include <thread>
#include <iostream>

#include "../list_ts.h"

list_ts<int> l0;
//WARN: can not pass list_ts in the function by value or by reference,
//  because the constructor of thread will copy its arguments ANYWAY, but list_ts is not allowed to be copied.
void insert_int_seq(int beg, int step, int end) {

    for (int i = beg; i < end; i += step) {
        l0.push_front(i);
    }
}

int main(int argc, char **argv) {

    int num_threads = 200;
    int num_nodes = 10000;

    std::vector<std::thread> threads(num_threads);
    for (int i = 0; i < num_threads; ++i) {
        threads[i] = std::thread(insert_int_seq, i*num_nodes, 1, (i+1)*num_nodes); 
    }

    std::chrono::milliseconds dura(2000);
    std::this_thread::sleep_for(dura);

    for (int i = 0; i < num_threads; ++i) {
        threads[i].join();
    }

    int total = 0;
    l0.for_each([&total](auto i) { total += 1;});
    
    std::cout << "Total " << total << std::endl;

    // dura = std::chrono::milliseconds(5000);
    // std::this_thread::sleep_for(dura);
}