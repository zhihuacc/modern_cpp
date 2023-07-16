#include <utility>
#include <iostream>

class Foo {
};

int main(int argc, char **argv) {
    
    Foo a1, a2, a3, a4;
    Foo &&b = std::move(a1);
    // Foo &c = std::move(a2);  // Error, can't init a lvalue ref with a rvalue. Compare this stmt with the assignment stmt to d.
    Foo &d = a3;
    d = std::move(a4);          // OK, can assign a lvalue ref with a rvalue
   
}