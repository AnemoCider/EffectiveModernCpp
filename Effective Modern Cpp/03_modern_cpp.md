# Moving to modern c++

## Table of Contents

- [Moving to modern c++](#moving-to-modern-c)
  - [Table of Contents](#table-of-contents)
  - [() vs {} when creating objects](#-vs--when-creating-objects)
    - [Idea of {}: uniform initializer](#idea-of--uniform-initializer)
      - [Default initialization of non-static data members](#default-initialization-of-non-static-data-members)
      - [Initialization of uncopyable objects](#initialization-of-uncopyable-objects)
    - [{} prohibits narrowing conversion among built-in types](#-prohibits-narrowing-conversion-among-built-in-types)
    - [Deal with most vexing parse](#deal-with-most-vexing-parse)
    - [Initializer list confusion](#initializer-list-confusion)
      - [Auto](#auto)
      - [Overloading](#overloading)
  - [Prefer nullptr to 0 or NULL](#prefer-nullptr-to-0-or-null)
  - [Prefer alias declarations to typedefs](#prefer-alias-declarations-to-typedefs)
    - [Alias looks more clean](#alias-looks-more-clean)
    - [Alias can be easily templated](#alias-can-be-easily-templated)
    - [Dependent type](#dependent-type)
    - [Stripping modifiers](#stripping-modifiers)
  - [Prefer scoped enums to unscoped](#prefer-scoped-enums-to-unscoped)
    - [Reduce variable name pollution](#reduce-variable-name-pollution)
    - [scoped enums are more strongly typed](#scoped-enums-are-more-strongly-typed)
    - [scoped enums can be forward-declared](#scoped-enums-can-be-forward-declared)
      - [Reason](#reason)
    - [Case when unscoped enum is useful](#case-when-unscoped-enum-is-useful)
  - [Prefer deleted functions to private (Should be highlighted but I forget how to)](#prefer-deleted-functions-to-private-should-be-highlighted-but-i-forget-how-to)
    - [Advantage](#advantage)
    - [Prevent conversion](#prevent-conversion)
    - [In template specialization](#in-template-specialization)
  - [Declare overriding functions override](#declare-overriding-functions-override)
    - [Requirements for overriding](#requirements-for-overriding)
    - [Reference qualifiers](#reference-qualifiers)
  - [Prefer const\_iterators to iterators](#prefer-const_iterators-to-iterators)
    - [Non-member cbegin, cend](#non-member-cbegin-cend)
  - [Declare functions noexcept is they won't emit exceptions](#declare-functions-noexcept-is-they-wont-emit-exceptions)
    - [Advantage](#advantage-1)
    - [An example - move in push\_back](#an-example---move-in-push_back)
    - [Conditionally noexcept](#conditionally-noexcept)
    - [noexcept is a hard task](#noexcept-is-a-hard-task)
    - [functions that should not emit exceptions](#functions-that-should-not-emit-exceptions)
  - [Use constexpr whenever possible](#use-constexpr-whenever-possible)
    - [How to use constexpr functions](#how-to-use-constexpr-functions)
    - [literal types](#literal-types)
  - [Make const member functions thread safe](#make-const-member-functions-thread-safe)
    - [mutable](#mutable)
    - [thread safety](#thread-safety)
      - [Modification with mutex](#modification-with-mutex)
      - [Modification with atomic](#modification-with-atomic)
    - [Takeaway](#takeaway)
  - [Understand special member function generation](#understand-special-member-function-generation)
    - [Default](#default)
    - [Copy](#copy)
    - [Move](#move)
    - [Rule of Three](#rule-of-three)
    - [Upgrade your code](#upgrade-your-code)
    - [Move and copy](#move-and-copy)
    - [Templated ctors](#templated-ctors)


## () vs {} when creating objects

### Idea of {}: uniform initializer

i.e., {} can (ideally) be used to initialize everything.

#### Default initialization of non-static data members

```Cpp
class myClass {
    int x {0}; // same as...
    int y = 0;
    int z(0); // Error!
};
```

#### Initialization of uncopyable objects

```Cpp
atomic<int> ai1 {0}; // same as...
atomic<int> ai2(0); 
atomic<int> ai3 = 0; // Error!
```

### {} prohibits narrowing conversion among built-in types

```Cpp
double x, y, z;
int sum1{x + y + z}; // Error!
```

### Deal with most vexing parse

```Cpp
myClass c(); // function, not instantiation
myClass c{}; // an object
```

### Initializer list confusion

#### Auto

```Cpp
auto x = {1, 2, 3}; // std::initializer_list<int>
```

#### Overloading

```Cpp
class myClass {
    myClass(int i, double d);
    myClass(int i, bool b);
    myClass(std::initializer_list<long double> il);
    operator float() const;
};
myClass c1(10, true); // first
{10, true}; // third
(10, 5.0); // second
{10, 5.0}; // third
myClass c5(c4); // Copy ctor
myClass c6{c4}; // c4 to float, then to long double, third
```

Moreover, compiler would stick with initializer list with narrow conversion that produces an error, rather than find a compatible one.
Only when there is no way to convert the arguments, the fallback will happen.
Also, no argument is interpreted as default ctor, rather than an empty init list.

## Prefer nullptr to 0 or NULL

nullptr is not a pointer. It is essentially std::nullptr_t which implicitly converts to all raw pointer types.
Although 0 and NULL can sometimes be deduced to be of a pointer type (and in those cases they become nullptr), it does not work when auto or template deduction presentes. Therefore, use nullptr!

## Prefer alias declarations to typedefs

### Alias looks more clean

```Cpp
typedef void (*FP)(int, string&);
using FP = void (*)(int, string&);
```

### Alias can be easily templated

```Cpp
template <typename T>
using myList = list<T, myAllocator<T>>;
myList<int> l;

struct myList {
    typedef list<T, myAlloc<T>> type; 
};
myList::type l;
```

### Dependent type

mylist::type may not be a type name because of the existence of template specialization.

```Cpp
template<>
class myList<float> {
    enum class myEnum {...}
    myEnum type;
};
```

Though there is no confusion when you explicitly specify the desired type, the problem does occur when you template it.

```Cpp
template<typename T>
class myC{
    typename myList<T>::type list;
}
```

In this case, type is a dependent type, i.e., it may not be a typename for some Ts. C++ requires you to add "typename" before a dependent type.
On the contrary, alias produces an independent type -- for any T, it is always a typename.

### Stripping modifiers

In C11, we can only use typedef way to strip away potential modifiers for tempates, e.g., 

```Cpp
std::remove_const<T>::type // const T -> T
```

but in C14 we have alias approaches

```Cpp
std::remove_const_t<T>
```

## Prefer scoped enums to unscoped

### Reduce variable name pollution

```Cpp
enum Color {R, G, B};

auto R = false; // error: R is public

enum class Color {R, G, B}; // private
```

### scoped enums are more strongly typed

```Cpp
enum Color {R, G, B};
Color c = R;
c < 2.0; // convert to double

enum class ...
c < 2.0; // Error
```

### scoped enums can be forward-declared

```Cpp
enum Color; // error
enum class Color; // OK, the definition can be separated from declaration.
```

#### Reason

Every enum has an underlying type.
Small number of values? Might choose char to represent.
But...

```Cpp
enum Status {
    good = 0,
    bad = 0xFFFFFFFF // chars can hardly represent the range!
};
```

Therefore, in C++98, we need the definition to determine the actual type.
Its drawback: introducing a new value requires the recompile the entire system!
In scoped enum, however, the default type is always int. You can also specify it:

```Cpp
enum class Status : std::uint32_t;
```

### Case when unscoped enum is useful

When tuple and get are involved.

```Cpp
get<1>(myEnum); // scoped
get<item1>(myEnum); // unscoped
```

## Prefer deleted functions to private (Should be highlighted but I forget how to)

MAY OCCUR IN INTERVIEW!!!

How to disable the automatically created functions? E.g., we don't want stream to be copied constructed.

C98:

```Cpp
class basic_ios {
    private:
        basic_ios(const basic_ios&); // private it and not implementing it
}
```

C11:

```Cpp
... {
    public:
        basic_ios(...) = delete;
}
```

### Advantage

Even member and friend functions cannot access it.
Explicitly warns that this function is deleted. Otherwise, it is technically only private.

### Prevent conversion

```Cpp
bool isGood(int number);
isGood(1.0f); // OK
bool isGood(char) = delete;
bool = delete;
double = delete; // this will disable 
// both float and double conversion,
// because float will be converted into 
// double before to int.
```

### In template specialization

```Cpp
template<typename T>
void procPt(T* pt);
```

void* and char* are often treated differently, so...

```Cpp
template<>
void procPt<void>(void*) = delete;
```

Template specializations must be written at namespace scope, thus

```Cpp
class myC {
public:
    template
    void procPt(T* pt);
private:
    template<>
    void procPt<void>(void*); // Error!
}
template<>
void myC::procPt<void>(void*) = delete; // Good
```

## Declare overriding functions override

### Requirements for overriding

- Base class function virtual
- Same name for base and derived functions, except destructors. Note that constructors should also follow this, however constructors cannot be virtual.
- Identical parameter type
- Identical constness of the functions
- Compatible return type and exception specification. Other wise you get an error: return type is not identical nor covariant.
- In C++11 and after, reference qualifiers must be identical. These are different functions.

```Cpp
public:
    void myFunc() &;
    void myFunc() &&; // called by rvalue object
```

Use override to ensure you correctly do the overriding.

### Reference qualifiers

Qualifier overloading of member functions requires the functions to both have or lack ref-qualifiers. Therefore, this will not compile:

```Cpp
vector<int> getData() {
    return data;
}
vector<int> getData() && {
    return data;
}
```

The use of qualifier overloading is to specifically deal with rvalue objects, for example:

```Cpp
auto data1 = myObject.data(); // lvalue, copy the data
auto data2 = makeObject().data(); // rvalue, we want to move the data.
```

## Prefer const_iterators to iterators

The idea is simple: using const is preferrable, therefore, using const_iterators is preferrable than iterators as well. However, there is a detail to take care of.

### Non-member cbegin, cend

container::cbegin() gives a const iterator. However, cbegin may not always be a member function, e.g., for build-in arrays. In this case, we should use std::cbegin to get the cbegin of the container. Interestingly, this is a C++14 feature, because they added std::begin, but forgot cbegin in C++11.

## Declare functions noexcept is they won't emit exceptions

### Advantage

It's part of the interface design, just like const.
It would allow compiler to generate better object code. In a noexcept function, optimizers need not to keep the runtime stack in an unwindable state, nor must they ensure that objects are destroyed in the inverse order, should an exception leave the function.

### An example - move in push_back

Consider vector push_back. In C++98, when the capacity expands, elements are copied. Therefore, push_back offers strong exception safety guarantee -- the original vector won't be affected if an error occurs during copying. However, if in C++11, we want to move the elements, such guarantee would be violated, and we may not be able to restore the original state when an error occurs during copying. Therefore, we cannot simply replace copy operations in puch_back to move UNLESS we know the move operations won't emit exceptions.
Therefore, if the move operation is nonexcept, we can do the replacement safely, optimizing the performance.

### Conditionally noexcept

std::swap of arrays:

```Cpp
template <class T, size_t N>
void swap(T (&a)[N], T (&b)[N]) noexcept(noexcept(swap(*a, *b))); // ref to array
```

is noexcept iff the swap of elements is noexcept.

### noexcept is a hard task

- you have to ensure it will not emit any exception in the future
- most functions are exception-neutral, for they invoke other functions which may emit exception, which are subsequently passed through.

Therefore, we don't use noexcept unless it would be a huge optimization, and...

### functions that should not emit exceptions

E.g., delete, delete[], and destructors. They are implicitly noexcept even if for user-defined ones. To change this, you should declare it noexcept(false).

## Use constexpr whenever possible

Constexpr objects are const, and are known at compile time. Constexpr functions generate constexpr values when called with compile-time constants, otherwise they produce runtime values.

### How to use constexpr functions

E.g., we have

```Cpp
constexpr int pow(int base, int exp) noexcept {
    ...
}
```

we can then utilize it to initialize arrays:

```Cpp
constexpr auto exp = 5;
std::array<int, pow(3, exp)> myArr;
```

Using it with runtime arguments is also valid, in this case, it produces runtime values.

### literal types

Literal types are types that can have values determined during compilation, including user-defined classes that satisfy this requirement. Constexpr functions are limited to taking and returning literal types.

## Make const member functions thread safe

### mutable

mutable keyword allows member to be changed by const member functiosn.

```Cpp
class A {
public:
    mutable int cache {0};
    mutable bool cached {false};
    void calculate() const {
        if (!cached) {
            cache = doSomeThing(); // very costly function
            cached = true;
        }
        return cache;
    }
};
```

### thread safety

Suppose we have two paralled operations:

auto val1 = a.calculate();
auto val2 = a.calculate();

calculate() is a const function, so we may assume these operations are thread safe. However, they are actually not, for they are simultaneously reading and writing the same memory without synchronization, which is defined to be a data race.

#### Modification with mutex

add the following:

```Cpp
mutable std::mutex m;
void calculate() const {
    std::lock_guard<std::mutex> g(m); // lock mutex
    ...
}
```

#### Modification with atomic

use atomic instead of mutex:

    mutable std::atomic<bool> cached {false};
    mutable std::atomic<int> cache;

Scenario 1:

    // check if cached
    // if not cache, calculate
    // and set cached = true

problem: both threads will check if cached, and both get false, so both will have to calculate.

Scenario 2:

    // check if cached
    // if not, set cached = true
    // and calculate

problem: this won't even work. One thread will set cached to true before calculating and writing the value. Then, the other see it true, and directly return the uninitialized value.

Conclusion: Use mutex when manipulation involving 2 or more variables as a unit occurs!

### Takeaway

Make const functions thread safe, such that they can be used in concurrent contexts.

## Understand special member function generation

### Default

Default ctor is only generated when no user-defined ctor is present.

### Copy

The two copy operations are independent, i.e., declaring one won't stop the compiler from generating the other. Note that the auto-generation only happens if it is needed, i.e., the code requires one. 

In addition, if any move operation is explicitly declared, no default copy operation will be generated. See below for the reason.

### Move

Default move constructors perform memberwise std::move. For members who don't support move, e.g., C++98 legacy, it is copied. Note that std::move doesn't guarantee the object to be moved.

The move operations are dependent. Declaring one prevents generating the other. The reason behind is that when you write your own move ctor, it indicates that the default way is not suitable for some of the members, so the other default one should not be applied as well.

In addition, when any copy operation is explicitly declared, no move operations will be generated. The reason is that if memberwise copy is not suitable, then neither is memberwise move.

Destructor will affect default move. See below.

### Rule of Three

If you declare any of a copy ctor, copy assignment, or destructor (the big three), you should declare all three to properly manage the resources. Therefore, if the dtor is explicitly declared, the default copy operations should not present. However, this is not the real case, because C++98 didn't adopt this, and we don't want to break legacy code... 
But, we can apply the rule for move operations. Therefore, if dtor is explicitly declared, no move operation will be generated.

### Upgrade your code

Use "= default" to explicitly generate the default operations, because C++11 begins to deprecate the C++98 conventions, i.e., declaring dtor may suppress the default copy.

### Move and copy

If the object is not move-enabled, std::move will cause copies to be made. Therefore, adding a destructor to a class may cause move operations on the object essentially become copy!

### Templated ctors

Templated operations won't affect the default behavior.

```Cpp
    class A {
        template<typename T>
        A(const T& rhs); // This can be instantiated to A(const A& rhs)
    };
```

In the above example, the copy and move operations will still be created automatically.
