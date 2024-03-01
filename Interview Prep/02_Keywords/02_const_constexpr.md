# 02 const, constexpr

## const

### pointer/ref to const

The object at that memory shall not be modified. This applies to all its members as well.
To overcome this: declare the member to be `mutable`.

Useful but maybe incorrect understanding: `const` applies to the left. If nothing is to the left, applies to the right. When we use `auto`, `const` applies to `auto` as a whole.

### auto, const, and pointer

```Cpp
class A {};
const A a;

auto b = a; // b is non-const
auto& c = a; // c is a const ref
auto p = &a; // p is a pointer-to-const
const auto p = &a; // p is a const pointer-to-const

const auto d = new A(); // const pointer, not pointing to a const
const auto* e = new A(); // pointer to const, not const pointer
```

## constexpr

### Applies to variable

The variable is const, and is a compile-time constant.

This would allow:

```Cpp
class A {
    public:
        // static int a = 10; error!
        // these are compile-time constants
        // we don't actually need to allocate memory for them
        const static int a = 10;
        constexpr static int b = 10;
}
```

Therefore:

```Cpp
int b = A::a; // ok, a is inlined
auto p = &A::a; // linker error! Can only take those initialized outside of the class.
```

### Applies to function

The function is computed at compile-time if all parameters are compile-time constant.