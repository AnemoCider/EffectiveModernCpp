#include <utility>
#include <iostream>

template<typename T>
decltype(auto) move(T&& param) { // universal reference
    using ReturnType = std::remove_reference_t<T>&&;
    auto b = &param;
    std::cout << b << '\n';
    return static_cast<ReturnType>(param);
}

int main() {
    auto&& a = move(1); // universal
    std::cout << &a << '\n';
    // essentially, there will be a lvalue created from 2, 
    // and moved to an rvalue, and finally referred to by b
    int&& b = 2; 
    std::cout << &b << '\n';
}