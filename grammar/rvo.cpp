#include <iostream>
#include <string>

class Base {
public:
    Base(int id):_id(id) {
        std::cout << "Base cstr: " << this << ", id = " << _id << std::endl;
    }
    Base(const Base &b):_id(b._id) {
        std::cout << "Base copy cstr: " << this << ", id = " << _id << std::endl;
    }
    virtual ~Base() {
        std::cout << "Base dstr: " << this << ", id = " << _id << std::endl;
    }
    int _id;
};

class Derived1 : public Base {
public:
    std::string name;
    Derived1(int id): Base(id) {
        std::cout << "Derived1 cstr: " << this << std::endl;
    }
    virtual ~Derived1() {
        std::cout << "Derived1 dstr: " << this << std::endl;
    }
};

Base create_base(int id) {
    Base obj(id);
    std::cout << "obj is constructed: " << &obj << std::endl;
    obj._id += 100;
    return obj;
}

int main(int argc, char **argv) {
    Base b = create_base(100);
    std::cout << "b will be destructed: " << &b << std::endl;
}