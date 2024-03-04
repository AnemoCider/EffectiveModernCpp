# 05 Rvalue References, Move Semantics, and Perfect Forwarding

## Understand std::move and std::forward

### std::move

move() returns the rvalue reference cast of an object.

```Cpp
template<typename T>
decltype(auto) move(T&& param) {
    using ReturnType = remove_reference_t<T>&&;
    return static_cast<ReturnType>(param);
}
```

rvalues are candidates for moving, so move() essentially attemps to make the object eligible to be moved from.

Why "Attemps to":

```Cpp
class string {
    string(const string& rhs);
    string(string&& rhs); // Note that move ctor can seldomly be const, because they usually modifies the object.
};
void moveString(const std::string text) {
    std::string moved(std::move(text)); // get const string&&, cannot call move ctor, copy instead
}
```

### std::forward

Idea: all parameters are lvalues. If we want to forward a param of rvalue reference type to another function, we can't do it directly. 

std::forward casts a parameter to an rvalue iff its argument was initialized with an rvalue. (More on this in reference collapsing)

## Distinguish universal references from rvalue references

There is no "universal references". But we can pretend there is and use it (so-called "abstractions")

### What is a universal reference

Examples:

```Cpp
auto&& var2 = var1;

template<typename T>
void f(T&& param);
```
These can bind to either rvalue ref or lvalue ref, even with const, volatile, etc.

### Rule of universal references

- its type is being deduced

No deduction, so no universal reference:

```Cpp
template<class T, class Allocator = allocator<T>>
class vector {
public:
    void push_back(T&& x); // not universal reference
};
```

The vector will be instantiated with a type, so the type is known when the function is called.

- it is in the form of T&&, or auto&&

e.g., 

```Cpp
auto func = [](auto&& param) {};
template<class... Args>
void emplace_back(Args&&... args);
```

## Use std::move on rvalue refs, std::forward on universal refs

move may finally lead to the invalidation of the object.