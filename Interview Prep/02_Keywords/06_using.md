# 06 using, ::

## using

Basic usages:

```Cpp
using std::cout;
// derived can use base constructor
class Derived : Base {
public: 
    using Base::Base;
};
```

Type alias, alias template:

```Cpp
template<class T>
struct Alloc{};

template<class T>
using Vec = vector<T, Alloc<T>>;
Vec<int> v;
```

## :: scope resolution operator

- ::name, global scope
- class::name, namespace::name