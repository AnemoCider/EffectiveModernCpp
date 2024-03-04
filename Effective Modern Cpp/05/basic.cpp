#include <iostream>

using namespace std;

void f(int&& a) {
    int b = a;
}

int main() {
    int a = 1;
    f(static_cast<int&&>(a));
}
