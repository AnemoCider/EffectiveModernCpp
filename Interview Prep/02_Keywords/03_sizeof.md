# 03 sizeof

## Usage

- For type: `sizeof(type)`
- For variable: `sizeof var` or `sizeof(var)`
- When applying to arrays, they won't decay
- Cannot apply to function name (it won't even try to decay)

## Results

returns `size_t` (`unsigned int`)

### pointers

Depends on the compiler. Typically 8 on 64-bit os.

### Primitive types

`char`: must be 1
`int`: usually 4

### struct

Memory is aligned to be cache friendly.

suppose the data is aligned by 4 bytes

padding between members: aligns with 4 bytes and the memory of the member to be placed
overall: aligns: aligns with both 4 bytes and the max memory of member

```Cpp
{}; // 1. consider an array of empty struct:
// If the size of each is 0, all objects will have the same address!
{char a}; // 1
{char a; int b;} // 8. 3 bytes padding between a and b.
```

### class

Basically the same as struct.

Note that:

```Cpp
{}; // 1
// static won't affect its size
// non-virtual functions do not affect sizeof, as they are stored in data segment
// virtual functions (however many) induces a single vptr, so class size increases by sizeof(pointer)

// suppose A, B are virtual
class C: public A {}; // 8
class C: public A, public B {}; // 16
class B: public A {}; class C: public B {}; // both are 8
```