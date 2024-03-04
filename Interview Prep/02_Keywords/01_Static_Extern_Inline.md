# 01 Static, Extern, Inline

## Static

### Static Member Variables

- They won't increase the class's size (e.g., `sizeof`)
- Stored in static segment
- Require initialization outside of the class (and only once within all source files)

### Static Global Variables

- Visible only within the current file (more precisely, "translating unit")
  - Note: All other files that include the current file will each have an independent static variable, which can lead to confusion.
- Same thing for static global functions

### Static Local Variables

- Its memory is allocated the first time the definition is encountered
- It is stored in static segment, but is visible only to the local scope

## Extern

ORD: One definition rule.

### Extern Variables

Defining vs Referencing declaration:

Referencing declaration only occurs when the variable is "declared" but not "definition".

```Cpp
extern int a; // defined elsewhere. No memory allocation. Linker will resolve it.
extern int a = 0; // defined here, memory allocated.
```

### Extern Functions

Function declarations are by default extern.

```Cpp
int myFunc();
// equivalent to
extern int myFunc();
```

## extern "C"

Usage:

```Cpp
extern "C" {
    void foo();
}
```

which makes the (C++) function have a C linkage (i.e., the compiler will not apply name mangling to cope with function overloading). The actual implementation is in a C object file, and we want to use that in our C++ program.

### An example

In a C header file:

```C
#ifdef __cplusplus
extern "C" {
#endif

// C code here

#ifdef __cplusplus
}
#endif
```

Now a C++ source file can include and use it.

## Inline

In modern, C++, `inline` has nothing to do with whether the compiler actually makes the function inline.

Inline can apply to both functions and variables.

- The definition of the inlined function/variable must be reachable in the same translating unit
- For non-static inline:
  - The function/object may be defined in multiple translating units (but still once in a unit). In this case, `inline` tells the linker that it can pick any definition in the final program. Therefore, all definitions must be identical.

## Mixure of directives

### Function

Say we have a function declared globally in the header file.

No directive: by default, `extern`. There should be exactly one definition in any source file. Use this to define global function in the header file.

`static`: each source file should define it separately. You can define it in the header file, but each source file will create and use an extra copy.

`inline`: There can be one or more definitions across the program. They should be identical.

`static inline`: same as `static`.

### Variable

`extern`: the most appropriate way to declare a variable in the header file.

`static inline` allows definition of static variables within a class.