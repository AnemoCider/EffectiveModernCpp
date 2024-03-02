# 04 new

## new

`new` is a C++ keyword. It cannot be overloaded.

### Implementation of `new`

- allocates a chunk of memory through `operator new`, which may be overloaded by the class
- calls the class constructor on that chunk of memory

```Cpp

class A {
    void* operator new(size_t sz) {
        void* storage = malloc(size);
        if (!storage) {
            std::bad_alloc ba;
            throw ba;
        }
        return storage;
    }
};
```

### placement new

Overloaded operator new.

```Cpp
class A {
    void* operator new(size_t size, void* p) {
        // construct at a pre-allocated space
    };    
};
```

Example:

```Cpp
template <class T, class... types>
void construct_in_place(T& Obj, types&&... Args) {
    ::new (static_cast<void*>(std::addressof(Obj))) T(std::forward<types>(Args)...);
}
```
