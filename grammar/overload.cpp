#include <iostream>
#include <cstring>
#include <string>

int funcA(int i) {
    std::cout << "funcA(int): " << i << std::endl;
    return i;
}

int funcA(int &i) {
    std::cout << "funcA(int): " << i << std::endl;
    return i;
}

int funcA(const int &i) {
    std::cout << "funcA(const int &): " << i << std::endl;
    return i;
}

template<typename T>
T const& max (T const& a, T const& b)
{
    std::cout << "max(T const &, T const &): " << std::endl;
    return b<a?a:b; 
}

// char const* == const char*: is a mutable pointer to const char.
// char *const: is a const pointer to mutable char.
char const* max (char const* a, char const* b)
{
    std::cout << "max(char const*, char const*): " << std::endl;
    return  std::strcmp(b,a) < 0  ? a : b;
}

// char const* const& max (char const* const&a, char const* const&b)
// {
//     std::cout << "max(char const* const&, char const* const&): " << std::endl;
//     return  std::strcmp(b,a) < 0  ? a : b;
// }

template<typename T>
T const& max3 (T const& a, T const& b, T const& c)
{
    std::cout << "max(T const &, T const &, T const &): " << std::endl;
    return max(max(a,b), c);
}


int main(int argc, char **argv) {
    // Case 
    // int n = 1;
    // funcA(n);  //Error, funcA(n) is ambiguous among all 3 funcA def.
    // funcA(10);    // Error, funcA(10) is ambiguous among funcA(int) and funcA(const int &).
                     // funcA(10) not match funcA(int &) since int& can't bind to literals which are constant.

    // Case
    // int i = 1;
    // const int &m = i;
    // funcA(m);  // Error, funcA(m) is ambiguous among funcA(int) and funcA(const int &).


    // Case
    // char const* s1 = "frederic";
    // char const* s2 = "anica";
    // char const* s3 = "lucas";
    // auto m2 = ::max3(s1, s2, s3);  // instantiated as max3<cost char*>. max in max3 is resolved to call char const* version,
    //                                //   which create a temporal value in the return stmt of max3 which is returned as ref from max3. This is not allowed.
}

// Conclusion: const and reference qualifier do not take advantages.