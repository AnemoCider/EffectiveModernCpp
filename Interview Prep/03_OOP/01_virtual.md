# 01 virtual

## vptr, vtable

### Usage

`virtual constructor`: NOT Ok. The vptr has not been created at this moment!
`static virtual`: NOT Ok. No `this` pointer. See below for why we need it.

### Mechanism

Calling a virtual function: `this->vptr->vtable->func`

(pseudo)
```cpp
void* __func[N] = { &Base::func1, ...} // vtable, stored in .rodata, created in compile time
const void** __vfptr = &__func[0] //vptr
```

- different instances of a certain class share a single vtable
- overriding replaces function pointers in vtable
e.g., `void* __func[N] = { &Derived::func1, ...}`

Complex scenes:

refer to [this blog](https://blog.twofei.com/496/)

## virtual inheritance

Use a pointer to access the members of the virtual base

## More Rules

### What cannot be virtual

- constructor, becasue vtable may not have been created
- static member function, because it is not bound to a specific object (no `this`)
- template member functions
  - compiler generate a fixed-size vtable
  - number of instance cannot be known before linking the whole program