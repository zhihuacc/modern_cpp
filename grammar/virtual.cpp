#include <iostream>

class Base1 {
protected:
    int data;
public:
    Base1(int d) : data(d) {};
    Base1() : Base1(0) {};
    virtual int Func1(int d) {
        if (this == nullptr) {
            std::cout << "Base1::Func1() : this = nullptr" << std::endl;
            return data;
        }
        std::cout << "Base1::Func1(): d = " << d << ", data = " << data << std::endl;
        return d;
    }
};

class Base2 {
protected:
    int data2;
public:
    Base2(int d): data2(d) {};
    Base2(): Base2(0) {};
    virtual int Func2(int d) {
        if (this == nullptr) {
            std::cout << "Base2::Func2() : this = nullptr" << std::endl;
            return data2;
        }
        std::cout << "Base2::Func2): d = " << d << ", data2 = " << data2 << std::endl;
        return d;
    }
};

class Derived1 : public Base1 {
private:
    int drv_data;
public:
    Derived1(int d1, int d2): Base1(d1), drv_data(d2) {};
    Derived1(): Base1(), drv_data(-1) {};
    virtual int Func1(int d) {
        if (this == nullptr) {
            std::cout << "Derived1::Func1() : this = nullptr" << std::endl;
            return d;
        }
        std::cout << "Derived1::Func1(): d = " << d << ", base1::data = " << data << ", drv_data = " << drv_data << std::endl;
        return d;
    }
};

class Derived2 : public Base1, public Base2 {
private:
    int drv_data;
public:
    Derived2(int d1, int d2, int d3): Base1(d1), Base2(d2), drv_data(d3) {};
    Derived2(): Base1(), Base2(), drv_data(-1) {}
    virtual int Func1(int d) {
        if (this == nullptr) {
            std::cout << "Derived2::Func1() : this = nullptr" << std::endl;
            return d;
        }
        std::cout << "Derived2::Func1(): d = " << d << ", base1::data = " << data << ", base2:data2 = " << data2 << ", drv_data = " << drv_data << std::endl;
        return d;
    }

    virtual int Func2(int d) {
        if (this == nullptr) {
            std::cout << "Derived2::Func2() : this = nullptr" << std::endl;
            return d;
        }
        std::cout << "Derived2::Func2(): d = " << d << ", base1::data = " << data << ", base2:data2 = " << data2 << ", drv_data = " << drv_data << std::endl;
        return d;
    }
};

typedef int (*VritualFunc1_T)(Base1*, int);
typedef int (*VirtualFun2_T)(Base2*, int);

int main() {
    Base1 bobj1(3);

    long *bobj1_base = reinterpret_cast<long*>(&bobj1);
    long *bobj1_vtbl_ptr = reinterpret_cast<long*>(*bobj1_base);
    std::cout << "bobj1 base1 = " << bobj1_base << std::endl;
    std::cout << "bobj1::vtble_ptr = " << bobj1_vtbl_ptr << std::endl;
    std::cout << "bobj1::typeinfo = " << reinterpret_cast<long*>(*(bobj1_vtbl_ptr-1)) << std::endl;
    std::cout << "bobj1::offset_to_top = " << reinterpret_cast<long>(*(bobj1_vtbl_ptr-2)) << std::endl;

    Base1 *b1_ptr1 = nullptr;
    Base1 *b1_ptr2 = nullptr;
    Base2 *b2_ptr1 = nullptr;
    Derived1 obj1(5, 6);
    Derived2 obj2(7, 8, 9);
    b1_ptr1 = static_cast<Base1*>(&obj1);
    b1_ptr2 = static_cast<Base1*>(&obj2);
    b2_ptr1 = static_cast<Base2*>(&obj2);
    std::cout << "obj1 base1 = " << b1_ptr1 << ", &obj1 = " << &obj1 << std::endl;
    std::cout << "obj2 base1 = " << b1_ptr2 << ", &obj2 = " << &obj2 << std::endl;
    std::cout << "obj2 base2 = " << b2_ptr1 << ", &obj2 = " << reinterpret_cast<long*>(&obj2) << std::endl;

    long *obj2_b2_vtbl_ptr = (long*)*(long*)b2_ptr1;
    std::cout << "obj2::base2_vptr_ptr = " << obj2_b2_vtbl_ptr << std::endl;
    std::cout << "obj2::base2_offset_to_top = " << *(obj2_b2_vtbl_ptr-2) << std::endl;
    VirtualFun2_T f2 = (VirtualFun2_T)*(obj2_b2_vtbl_ptr);
    // The first parameter type is Base2, need pass the pointer to obj2's Base2 subobject, instaed of &obj2.
    f2(b2_ptr1, 10);
}

