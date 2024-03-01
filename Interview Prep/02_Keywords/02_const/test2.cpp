
#include <iostream>

class A {
    public:
        // static int a = 10; error!
        // these are compile-time constants
        // we don't actually need to allocate memory for them
        const static int a = 10;
        constexpr static int b = 10;
};

int main() {
    int b = A::a;
    // auto p = &A::a;
    // std::cout << p << '\n';
}