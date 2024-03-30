#include <iostream>
#include <type_traits>

class A {
    public: 
        void* operator new(size_t sz) throw (std::bad_alloc){

        }
};

int main() {
    new A();
}

