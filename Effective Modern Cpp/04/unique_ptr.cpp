#include <iostream>
#include <memory>

using namespace std;

class A {
    int val;
};

class B : public A {
    int val1;
};

auto myDtor = [](A* a){
        cout << "delete A\n";
        delete a;
    };

template<typename... Ts>
    unique_ptr<A, decltype(myDtor)> 
    makeA(Ts&&... params)
    {
        unique_ptr<A, decltype(myDtor)> pA(nullptr, myDtor);
        if (true)
            pA.reset(new B(forward<Ts>(params)...));
        return pA;
    };

int main() {
    
    A* a = new A();
    auto ptr = makeA();
    ptr.reset(a);
    delete a;
}