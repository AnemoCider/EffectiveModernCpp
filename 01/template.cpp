#include <iostream>

using namespace std;

template<typename T>
void f(T x) {
    x = new int(5); // This is OK -- non-const pointer!
    *x = 10; // While this is not -- pointer to const int.
}

int main() {
    const int x = 1;
    const int* const p = &x; 
    f(p);
}