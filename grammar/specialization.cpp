#include <string>
#include <iostream>

template <typename T1>
class List {  
    // No template args list means it's primary class template. 
    // Primary class template does not allow template args list.
public:
    List() {std::cout << "List<T1>" << std::endl;}
    T1 t1;
    int n;
};


// template <typename T1>
// class List<T1, T1*> {  
//     // Template args list here should match parameters of primary template. 
//     // E.g., it can't have different number of args from that of parameters of primary template. 
//     // (not considering variadic templates here)
// public:
//     List() {std::cout << "List<T1>" << std::endl;}
//     T1 t1;
//     int n;
// };


template <typename T1, typename T2>
class List<T1* T2::*> { 
    // Template args list here should match parameters of primary template. 
    // E.g., it can't have different number of args from that of parameters of primary template. 
    // (not considering variadic templates here)
public:
    List() {std::cout << "List<T1* T2::*>" << std::endl;}
    T1 t1;
    T2 t2;
};

int main(int argc, char **argv) {
    List<std::string> list;
}