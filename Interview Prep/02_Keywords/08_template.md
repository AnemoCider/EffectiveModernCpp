# 08 Advanced Templates

## Type Traits

Idea: get some information of or manipulate the `type`

### Example: `is_integral`

```Cpp
template<typename _Tp>
    struct is_integral
    : public __is_integral_helper<typename remove_cv<_Tp>::type>::type
    { };

template<typename>
    struct __is_integral_helper
    : public false_type { };

template<>
    struct __is_integral_helper<bool>
    : public true_type { };

template<>
    struct __is_integral_helper<char>
    : public true_type { };

// manually specifies more...

/// The type used as a compile-time boolean with true value.
typedef integral_constant<bool, true>     true_type;

template<typename _Tp, _Tp __v>
    struct integral_constant
    {
      static constexpr _Tp                  value = __v;
      // ...
    };
```

### Example: `remove_cv`

```Cpp
/// Alias template for remove_cv
template<typename _Tp>
    using remove_cv_t = typename remove_cv<_Tp>::type;

template<typename _Tp>
    struct remove_cv
    {
      typedef typename
      remove_const<typename remove_volatile<_Tp>::type>::type     type;
    };

/// remove_const
template<typename _Tp>
    struct remove_const
    { typedef _Tp     type; };

// specialized for const type
template<typename _Tp>
    struct remove_const<_Tp const>
    { typedef _Tp     type; };
```

## Type Erasure

Idea: we have several classes, all implementing the function `run`. How to wrap them in one abstract class so that we can, e.g., store them in a vector?

### Attempts

#### pure virtual interface

```Cpp
class CanRun {
public:
    virtual void run() = 0;
};

class A: public CanRun {
    // ...
}
```

This absolutely does not work, because we should not control the implementation of the classes!

#### template function

```Cpp
template<class T>
void doRun(T* obj) {
    obj->run();
}
```

This doesn't work either.

- This is a function interface, so we cannot put the objects in one vector.
- Anyone uses this function should be templated as well, to pass T.

#### simulating `concept`

```Cpp
// concept
class CanRun {
public:
    virtual void run();
};

template<typename T>
class Wrapper : public CanRun {
    const T* _obj;
public:
    Wrapper(const T* obj) : _obj(obj) {}
    void run() override {_obj->run();}
};
```

### the actual `concept`

```Cpp
template<typename T>
concept Runnable = requires(T obj) {
    // requirements that T must satisfy
    // here it must return a void
    { obj.run() } -> std::same_as<void>
    // or, just can run
    { obj.run() };
}

template<Runnable T>
class Wrapper {
    // ...
    auto run() {return _obj.run();}
};
```

Benefits:

- Clear meaning
- Most importantly, clear error messages!