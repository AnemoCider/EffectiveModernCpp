#include <iostream>

using namespace std;

template<typename T>
void f(int x) {}

class A {

};

int main() {
    decltype(f<int>) x;
    A a;
    A * const b = &a;
    decltype(auto) c = b;
}