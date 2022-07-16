#include <iostream>
#include "list_ts.h"
#include "queue_ts.h"

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

    // std::shared_ptr<int> found = l0.find_first_if([](auto i){ return i == 2;});
    // std::cout << "found: " << *found << std::endl;

    // l0.remove_if([](auto i) {return i == 2;});
    // l0.remove_if([](auto i) {return i == 6;});
    // l0.remove_if([](auto i) {return i == 3;});
    // l0.remove_if([](auto i) {return i == 4;});
    // l0.remove_if([](auto i) {return i == 4;});
    // l0.for_each([](auto i) {std::cout<< i << std::endl;});
    // std::cout << "------" << std::endl;

    queue_ts<int> queue;

    queue.push(2);
    queue.push(3);
    queue.push(4);
    queue.push(-1);
    queue.push(6);

    std::shared_ptr<int> n = queue.try_pop();
    std::cout << "Pop " << *n << std::endl;
    std::cout << "------" << std::endl;

    n = queue.wait_pop();
    std::cout << "Pop " << *n << std::endl;
    std::cout << "------" << std::endl;

    n = queue.wait_pop();
    std::cout << "Pop " << *n << std::endl;
    std::cout << "------" << std::endl;

    n = queue.try_pop();
    std::cout << "Pop " << *n << std::endl;
    std::cout << "------" << std::endl;

    n = queue.try_pop();
    std::cout << "Pop " << *n << std::endl;
    std::cout << "------" << std::endl;

    n = queue.try_pop();
    std::cout << "Pop " << *n << std::endl;
    std::cout << "------" << std::endl;

}