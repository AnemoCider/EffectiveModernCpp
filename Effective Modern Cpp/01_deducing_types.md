# 01 Deducing Types

## Template Deduction

Consider:

    template<typename T>
    void f(ParamType param);
    f(expr);

### ParamType is a reference

Rule: Ignore expr's reference-ness, but keep const (after the ignoring), then match against ParamType to get T.

Example:

    void f(T& x);
    int x = 1;
    const int cx = x;
    const int& rx = x;
    f(x); // int, int&
    f(cx); // int, const int&
    f(rx); // int, const int&

Note: the `const`-ness is kept such that this is valid:

    const MyClass c;
    auto &r = c; // valid, r is const myClass&

### Universal Reference

Only in this case that T can be deduced to a reference.

- If expr is an lvalue, both T and ParamType are deduced to be lvalue references.
- If expr is an rvalue, then the normal case.

Examples:

```Cpp

template<typename T>
void f(T&& param);

int x = 1;
const int cx = x;
const int& rx = x;

f(1); // T is int, ParamType is int&&
f(x); // T is int&, ParamType is int&&& ==> int&
f(cx); // T is const int&, ParamType is const int&&& ==> const int&
f(rx); // T is const int&, ParamType is const int&&& ==> const int&
```

Idea is simple: We don't want to convert an lvalue to rvalue (this is `move`'s job).

### Pass-By-Value

Besides ignoring the input's reference-ness, ignore the const in addition. 

Example:

    template<typename T>
    void f(T param);
    const char* const ptr = "hello";
    f(ptr); // const char

    const char& rx = 'h';
    f(rx); // char

### <mark>Array</mark>

    void f(T param); // int*, int*
    void f(T* param); // int*, int
    void f(T& param); // ???

First guess: If the array decays into a pointer, passing a int* as argument would make T a reference to int*.

Real case: the array type is preserved, i.e., T& becomes int (&)[]. E.g., sizeof inside the function can now correctly calculate the space the array takes.

Example:

    template<typename T, std::size_t N>
    void f(T (&)[N]);

### <mark>Function Arguments</mark>

Another thing that may decay into pointers: Function types.

    void someFunc(int, double);

    template<typename T>
    void f(T param);
    f(someFunc); // void (*)(int, double)

    void f(T&, param);
    f(someFunc); // void (&)(int, double)

## Auto Type Deduction

C11 auto works almost the same like template, except that it accepts std::initializer_list.

    auto x = {1, 2, 3}; // std::initializer_list<int>
    template<typename T>
    void f(T param);
    f({1, 2, 3}); // Error, cannot deduce type
    void f1(std::initializer_list<T> initList);
    f1({1, 2 ,3}); //works

For C14, auto return type is essentially template deduction, therefore:

    auto getList() {
        return {1, 2, 3}; // Error, cannot deduce. Template deduction doesn't work for init list.
    }

The same things happens for lambda auto (because it uses template):

    vector<int> v;
    auto resetV = [&v](const auto& newValue) { v = newValue; }; 
    resetV({1, 2, 3}); // can't deduce

## decltype

Normally, gets the type of the variable/expression. It will keep reference or const:

    decltype(v[0]) // type is int&

Common use is declaring function templates when return type depends on parameter:

    // C11, only deduces return type for lambda
    template<typename Container, typename Index>
    auto getIndex(Container& c, Index i) -> decltype(c[i]) { // trailing return type syntax
        return c[i];
    }
    // C14, can deduce for all functions, but...
    auto getIndex(Container& c, Index i) {
        return c[i]; // auto deduction, returning a lvalue instead of ref!
    }
    // C14, with decltype
    decltype(auto) getIndex(Container& c, Index i) { // apply decltype rule.
        return c[i];
    }

### Rules and Edge cases of decltype

For decltype(exp):
1. exp is a variable or member access expression, return exactly the same type as exp
2. exp is a function call, return the return type
3. otherwise, if exp is an lvalue, return reference type of the lvalue, else, same as 1.

Therefore, an edge case is:

    decltype(auto) f1() {int x = 0; return x;} // return int
    decltype(auto) f2() {return (x);} // return int& !

