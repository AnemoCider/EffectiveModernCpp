#include <iostream>

class A {
    public:
        virtual int get() {return 1;}
        virtual int gettwo() {return 2;}
};

class C: public A{
    public:
        virtual int get() {return 1;}
        virtual int gettwo() {return 2;}
};

class B : public C{
    int get() override {return 3;}
};

int main() {
    char arr[1];
    std::cout << sizeof (B) << '\n';
}