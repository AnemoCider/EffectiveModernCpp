#include <iostream>
#include <memory>

using namespace std;

class A {
    int a;
};

int main() {
    auto sPtr = make_shared<A>();
    auto wPtr = weak_ptr(sPtr);
    // int a = wPtr->a;
}