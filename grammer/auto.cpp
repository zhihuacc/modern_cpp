#include <typeinfo>
#include <iostream>
#include <map>
#include <string>


int main(int argc, char **argv) {

    int x = 3;

    auto *a = &x; // auto = int
    auto b = a;  // auto = int*
    std::cout << "a is " << typeid(a).name() << std::endl;
    std::cout << "b is " << typeid(b).name() << std::endl;

    auto &c = x;  // c is int&
    auto d = c;   // NOTE: d is int, NOT int&
    std::cout << "c is " << typeid(c).name() << std::endl;
    std::cout << "d is " << typeid(d).name() << std::endl;

    const auto e = x;  // e is const int
    auto f = e;   // NOTE: f is int, NOT const int
    std::cout << "e is " << typeid(e).name() << std::endl;
    std::cout << "is_const<decltype(e)>::value: " << std::is_const<decltype(e)>::value << std::endl;
    std::cout << "f is " << typeid(f).name() << std::endl;
    std::cout << "is_const<decltype(f)>::value: " << std::is_const<decltype(f)>::value << std::endl;

    f = 1;        // no error 

    const auto &g = x;  // g is const int&
    auto &h = g;        // NOTE: h is const int&, it's different from f
                        //   when auto is with & or *, const is preserved,
                        //   while not with & or *, const is removed.
    auto i = g;         // i is int, NOT const int or const int&
    std::cout << "g is " << typeid(g).name() << std::endl;
    std::cout << "h is " << typeid(h).name() << std::endl;
    std::cout << "i is " << typeid(i).name() << std::endl;
    std::cout << "is_lvalue_reference<decltype(i)>::value: " << std::is_lvalue_reference<decltype(i)>::value << std::endl;
    std::cout << "&i == &g: " << (&i == &g) << std::endl;   // 0, i is not ref to g
    // h = 1;     // NOTE: complier error: h is const
    i = 1;

    const auto *j = &x;  // i is const int*
    auto *k= j;          // j is const int*
    std::cout << "j is " << typeid(j).name() << std::endl;
    std::cout << "k is " << typeid(k).name() << std::endl;

    // *k = 1;    // NOTE: complier error: *j is const
}

// auto does not automatically keep ref qualifier. If we need it, we need explicitly use "auto &"". See c, d.
// If auto is not with &, const qualifier is not kept. See i.
// If auto is with &, const is kept. Otherwise, may declare a non-const ref points to a const ref. See h.