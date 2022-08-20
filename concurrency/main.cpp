#include <iostream>
#include "list_ts.h"
#include "queue_ts.h"
#include "unordered_hashmap_ts.h"

int main(int argc, char **argv) {

    // list_ts<int> l0;

    // l0.push_front(2);
    // l0.push_front(3);
    // l0.push_front(4);
    // l0.push_front(-1);
    // l0.push_front(6);

    // l0.for_each([](auto i) {std::cout<< i << std::endl;});
    // std::cout << "------" << std::endl;

    // l0.remove_if([](auto i) {return i == -1;});
    // l0.for_each([](auto i) {std::cout<< i << std::endl;});
    // std::cout << "------" << std::endl;

    // int data;
    // bool found = l0.find_first_if([](auto i){ return i == 2;}, data);
    // if (found)
    //     std::cout << "found: " << data << std::endl;
    // else
    //     std::cout << "not found: " << std::endl;
    // std::cout << "------" << std::endl;

    // l0.remove_if([](auto i) {return i == 2;});
    // l0.remove_if([](auto i) {return i == 6;});
    // l0.remove_if([](auto i) {return i == 3;});
    // l0.remove_if([](auto i) {return i == 4;});
    // l0.remove_if([](auto i) {return i == 4;});
    // l0.for_each([](auto i) {std::cout<< i << std::endl;});
    // std::cout << "------" << std::endl;

    // queue_ts<int> queue;

    // queue.push(2);
    // queue.push(3);
    // queue.push(4);
    // queue.push(-1);
    // queue.push(6);

    // std::shared_ptr<int> n = queue.try_pop();
    // std::cout << "Pop " << *n << std::endl;
    // std::cout << "------" << std::endl;

    // n = queue.wait_pop();
    // std::cout << "Pop " << *n << std::endl;
    // std::cout << "------" << std::endl;

    // n = queue.wait_pop();
    // std::cout << "Pop " << *n << std::endl;
    // std::cout << "------" << std::endl;

    // n = queue.try_pop();
    // std::cout << "Pop " << *n << std::endl;
    // std::cout << "------" << std::endl;

    // n = queue.try_pop();
    // std::cout << "Pop " << *n << std::endl;
    // std::cout << "------" << std::endl;

    // n = queue.try_pop();
    // std::cout << "Pop " << *n << std::endl;
    // std::cout << "------" << std::endl;


    unordered_hashmap_ts<int, std::string> hashmap;
    hashmap.set(23, "zhihua");
    hashmap.set(24, "john");
    hashmap.set(89, "jane");
    hashmap.set(90, "jane1");
    hashmap.set(91, "jane2");
    hashmap.set(92, "jane3");
    hashmap.set(102, "david");

    bool found;
    std::string val;
    found = hashmap.get(92, val);
    if (found)
        std::cout << "Found " << val << std::endl;
    else 
        std::cout << "Not found" << std::endl;

    found = hashmap.get(1, val);
    if (found)
        std::cout << "Found " << val << std::endl;
    else 
        std::cout << "Not found" << std::endl;

    hashmap.del(92);
    found = hashmap.get(92, val);
    if (found)
        std::cout << "Found " << val << std::endl;
    else 
        std::cout << "Not found" << std::endl;

    hashmap.set(102, "DAVID");
    found = hashmap.get(102, val);
    if (found)
        std::cout << "Found " << val << std::endl;
    else 
        std::cout << "Not found" << std::endl;
    
}