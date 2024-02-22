#include <iostream>

using std::cout;

class Base {
public:
    int myFunc() {
        return 1;
    }
};

class Derived : public Base {
    int myFunc() {
        return 2;
    }
};

int main() {
    Derived d;
    Base &b = d;
    cout << b.myFunc() << '\n'; // 1
}