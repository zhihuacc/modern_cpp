#include <iostream>
#include <vector>

int main(int argc, char **argv) {
    std::vector<int> vec(5);
    for (const auto &v : vec) {
        std::cout << &v << std::endl;
    }
    
    std::cout << std::endl;

    for (int i = 0; i < vec.size(); i++) {
        std::cout << &vec[i] << std::endl;
    }
    return 0;
}