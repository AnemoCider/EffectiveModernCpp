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