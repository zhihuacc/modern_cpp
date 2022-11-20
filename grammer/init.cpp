
#include <string>
#include <iostream>

class Ada {
public:
    int a;
    std::string b;
};

class Bar {
public:
    int a;
    std::string b;

    Bar(int x, const std::string &y): a(x), b(y) {std::cout << "Bar ctr" << std::endl;}
};

class Car {
public:
    int a = 1;  // this makes Car not an aggregate type.
    std::string b;
};

int main(int argc, char **argv) {
    
    Ada a1 {1, "ada"};    // NOTE: Ada is an aggregate type. This stmt does not invoke constructors but use list init. 
    Bar b1 {1, "bar"};    // This stmt invoke the corresponding constructor
    // Car c1 {1, "car"};     // NOTE: Car is NOT an aggregate type due to "a = 1" in class def. This stmt invokes constructors.
}