#include <iostream>
#include <type_traits>


int main(int argc, char **argv) {

    int i = 0, j = 1;

    i, j = 2;  // This is an expression return j as lvalue reference
    (j) = 3;   // This is an expression return j as lvalue reference
    std::cout << "j = " << j << std::endl;

    std::cout << "is_lvalue_reference<i>: " << std::is_lvalue_reference<decltype(i)>::value << std::endl;  // 0
    // Similar to comma expression, (i) return lvalue reference
    std::cout << "is_lvalue_reference<i>: " << std::is_lvalue_reference<decltype((i))>::value << std::endl; // 1

}