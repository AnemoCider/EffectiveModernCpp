# 07 Cast

## dynamic_cast

- only applies to pointer to/ref of class objects
- supports down cast, but it checks whether it is valid

## static_cast

- no runtime check, therefore
- supports both downcast and upcast(unsafe)
- can do implicit conversion, and convert the other way

```Cpp
Base *b = new Base();
Derived *d = static_cast<Derived*>(b);

// enum to int
// int to enum
```

## reinterpret_cast

- convert between arbitrary pointers

## const_cast

- remove const. It enables you pass a const argument to non-const parameter, but if you really write to it, the result is ub.