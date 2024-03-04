# 05 explicit

## explicit

May apply to:

- constructor
- conversion function

It prevents implicit conversion or copy initialization, i.e., such conversions are not considered as they were.

```Cpp
struct A {
    /*explicit */A(int) {}
    /*explicit */A(int, int) {}
    /*explicit */operator bool() const {return true;}
};

// if commented, then it is not OK with explicit
A a = 1; // copy init
A a(1);
A a {1, 2};
A a = {4, 5}; // copy init
A a = (A)1;
if (a) {}
bool a1 = a; // copy init
A a1 = static_cast<bool>(a);
```

