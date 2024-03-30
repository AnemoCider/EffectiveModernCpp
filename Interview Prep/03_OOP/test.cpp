#include <iostream>

using namespace std;

class A {
    template<typename T>
    virtual void func(T& t) {
        cout << "haha\n";
    }
};