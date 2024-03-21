# 01 Static, Extern, Inline

## Table of Contents

- [01 Static, Extern, Inline](#01-static-extern-inline)
  - [Table of Contents](#table-of-contents)
  - [Basic](#basic)
    - [Storage Duration](#storage-duration)
    - [Linkage](#linkage)
    - [Variable linkage](#variable-linkage)
    - [static local variables](#static-local-variables)
  - [Static](#static)
  - [Extern](#extern)
    - [Extern Variables](#extern-variables)
    - [Extern Functions](#extern-functions)
  - [Inline](#inline)
    - [local objects in inline](#local-objects-in-inline)
    - [Implicit inline](#implicit-inline)
    - [static inline member](#static-inline-member)
    - [Inline on static members](#inline-on-static-members)
  - [Mixure of Specifiers](#mixure-of-specifiers)
    - [Function](#function)
    - [Variable](#variable)
  - [Function inlining](#function-inlining)
    - [inline vs macro](#inline-vs-macro)
    - [What may not be inlined](#what-may-not-be-inlined)
    - [Other problems of inline](#other-problems-of-inline)


## Basic

Two important propertis of a variable/function name.

### Storage Duration

- automatic: local, bound to an enclosing code block
  - i.e., they only live within the current pair of curly braces
- static: global, bound to the program
- dynamic: bound to `new` and `delete`

### Linkage

Linkage is a property that enables a `name` to go across its scope.

- no linkage: the `name` is only visible within its scope
- internal linkage: it can be referred to within its current translation unit
- external linkage: it can be referred to from scopes in other translation units
  - language linkage: variable/function with external linkage also has language linkage so that they can link translation units written in other languages

### Variable linkage

- static/const namespace scope variables are internal
- static functions are internal
- all others are external
  - particularly, inline (including implicitly inline) variables are external

### static local variables

variables declared at block scope with `static` specifier.

- they have `static` duration, but they are initialized the first time control passes through their declaration, and only once.
- initialization will attempt to happen again if the previous fail to do so (e.g., exception)
- UB if the initialization recursively enters the block
- thread safe: similar to `std::call_once` (essentially, atomically read/write to a flag)
- if the corresponding function is inlined, all inlined instances refer to the same variable

## Static

- namespace scope: internal linkage
- declare class member: a static member
- declare object: `static` storage duration

## Extern

ORD: One definition rule.

### Extern Variables

Defining vs Referencing declaration:

Referencing declaration only occurs when the variable is "declared" but not "definition".

```Cpp
extern int a; // defined elsewhere. No memory allocation. Linker will resolve it.
extern int a = 0; // defined here, memory allocated.
inline int a; // external linkage, but memory allocated here.
```

### Extern Functions

Function declarations are by default extern.

```Cpp
int myFunc();
// equivalent to
extern int myFunc();
```

## Inline

In modern, C++, `inline` is no more likely to serve as even a hint to the compiler. It now means "there can be multiple definitions".
Inline can apply to both functions and variables.

- The definition of the inlined function/variable must be reachable in the same translating unit
- For non-static inline:
  - The function/object may be defined in multiple translating units (but still once in a unit). In this case, `inline` tells the linker that it can pick any definition in the final program. Therefore, all definitions must be identical.

### local objects in inline

Local static objects and typedefs in all definitions are shared.

### Implicit inline

Function:

- A class/struct/union function is implicitly inlined, regardless of whether it's a member function or not.
- A deleted function is implicitly inline, because multiple deleted definitions can appear in multiple units.

Variable:

- static constexpr (on its first declaration) data memebr variable is implicitly inline.

### static inline member

Why we cannot initialize static objects within a class: because if we do this, each time we create a class, there will be a copy of the object, which is not allowed.
Why can we do it with `inline`: `inline` permits multiple copies, and the linker will pick one as the final instance.

### Inline on static members

## Mixure of Specifiers

### Function

Say we have a function declared globally in the header file.

No directive: by default, `extern`. There should be exactly one definition in any source file. Use this to define global function in the header file.

`static`: each source file should define it separately. You can define it in the header file, then each source file will create and use an extra copy.

`inline`: There can be one or more definitions across the program. They should be identical.

`static inline`: same as `static`.

`extern inline`: same as `inline`

### Variable

Nothing: external linkage

`const` or `static`: internal linkage

`inline`: external linkage. There can be multiple definitions. BTW, `inline const` has external linkage. This doesn't make a difference, declare the variable in the header file `const` would be fine as well.

`extern`: the most appropriate way to declare a variable in the header file.

`static inline`: allows definition of static variables within a class, but better not use it in namespace scope (they are contradictory!)

`extern inline`: allow multiple definitions. You can even initialize it in the header in place, and include the header file multiple times.

## Function inlining

Compiler performs function inlining by replacing a call of the function with its body, avoiding the overhead of function calls (placing data on stack and retrieving the result)

### inline vs macro

Similar effect, but:

- macros expand textually, so the identifiers refer to those at the point of call, and may vary within different context; for inline, they refer to those at the point of definition.

### What may not be inlined

- functions in a separate translation unit. The compiler can't see the code.
- functions with large code size. The final program might be extremely large.
- recursive functions, because too many copies may be generated. However, some compilers do inline the function up to a certain depth, and then do a function call. For tail-recursive, however, the depth doesn't increase, so they may be fully inlined.
- functions with loop, switch, etc. Think about the assembly:

    ```asm
    start_loop:
        // loop logic
        add i 1

    check_loop_condition:
        cmp i n
        jl start_loop // jump less
    ```

- function pointers

### Other problems of inline

- when code size becomes larger, cache coherency is reduced, and even page faults may be triggered frequently.
