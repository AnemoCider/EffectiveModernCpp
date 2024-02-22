# 01 Memory Model in C++

## Memory Layout

Code, Static Data, Heap, Stack.

### Static

It contains:

- Global variables declared outside of functions (need NOT be static)
- Static variables
- Static constants, e.g., string literals

It is divided into:

- Initialized data segment (`.data`)
- Uninitialized (`.bss`), all memories are initialized to 0

About the usage of `static`: [Static and Extern](../02_Keywords/01_Static_Extern_Inline.md)

### Code

Besides instructions, the `constant folder` (aka `constant propagation`) of compiler may replace variables as hardcoded values, and the corresponding data is stored in the code segment, rather than the static or stack.