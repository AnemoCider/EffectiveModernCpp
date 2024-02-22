# 04 Smart Pointers

## Preface

### Pitfalls of raw pointers

- Don't know whether a raw ptr points to an object or an array
- Don't know whether to destroy it after using
- Don't know how to destroy -- delete, delete[], or some other machanism?
- Hard to ensure that you destroy it only once along every path
- Don't know if a pointer dangles

### auto_ptr

deprecated feature from C98. C98 has know move semantics, so it's impossible to implement a real smart pointer. unique_ptr from C11 does everything auto_ptr does, and has some even better features.

## Use unique_ptr for exclusive-ownership resource management

It's move-only. Upon destruction, a non-null unique_ptr destroys its resource, which is delete the raw pointer inside by default.

### Custom deleter

```Cpp
    auto myDtor = [](A* a) {delete a};
    unique_ptr<A, decltype(myDtor)> uPtr;
```

### Factory function that returns unique_ptr

```Cpp
template<typename... Ts>
unique_ptr<A, decltype(myDtor)> 
makeA(Ts&&... params)
{
    unique_ptr<A, decltype(myDtor)> pA(nullptr, myDtor);
    if (/* derived type B should be created*/)
        pA.reset(new B(forward<Ts>(params)...));
    else if...
    else...
    return pA;
}
```

### Reset

raw pointer to unique_ptr conversion is prohibited, so we use reset.
Remark: What the author didn't mention is that reset essentially allows raw pointer to unique_ptr conversion by uPtr.reset(rPtr). If rPtr is deleted, then double free would occur. Therefore, always use reset(new ...)

### Performance

unique_ptr has the same size as a raw pointer when it doesn't include a custom deleter. 

#### State

Stateless function objects, e.g., a lambda expression with no captures. They incur no size penalty, so a captureless lambda is preferred than a function.

On the other hand, a function pointer as a custom deleter requires at least additional size of function pointer, whereas deleters with even more state can yield a unique_ptr of significant size.

### Work with arrays

There are two forms of unique_ptrs:

```Cpp
    unique_ptr<T> // for object, has no indexing operator.
    unique_ptr<T[]> // for array, has no dereferencing or ->.
```

which avoids ambiguity. However, we often prefer std::array, vector, or string than a unique_ptr to an array.

### Conversion to shader_ptr is easy

A factory function, however, doesn't know whether the resource is exclusive or not. In this case, it should return a unique_ptr, which can be subsequently cast to shared_ptr if the resource should be shared.

## Use shared_ptr for shared-ownership resource management

### Reference count

shared_ptr maintains a pointer to the object's control block (which contains a ref count), and destroys the resource when the last shared_ptr is destroyed.
As consequences:

- shared_ptr is twice as large as a raw ptr
- Memory for ref count must be dynamically allocated
- Increments/Decrements of ref count must be atomic for thread safety. Therefore, modifying ref count is relatively costly.

### Custom deleter

Compare:

```Cpp
auto myDtor = [](A* a) {delete a};
unique_ptr<A, decltype(myDtor)> uPtr(new A, myDtor);
shared_ptr<A> sPtr(new A, myDtor);
auto anotherDtor = ...
shared_ptr<A> anotherSPtr(new A, anotherDtor);
// This allows...
vector<shared_ptr<A>> vec {uPtr, anotherSPtr}
anotherSPtr = sPtr;
```

This is achieved by...

### Control Block

For each object managed by shared_ptr, there is a control block consisting of:

- Reference count
- Weak count
- Custom deleter, allocator, etc.

### Construction

Problem: You don't know whether the object already has a control block!
Rules:

A control block is always created when:

- make_shared manufactures a new object
- when shared_ptr is constructed from a unique_ptr(or auto_ptr)
- when created from a raw pointer

Therefore, constructing multiple shared_ptr from a raw pointer leads to undefined behavior.

#### Work with *this

Problem: An object cannot create a shared_ptr for itself, for it doesn't know whether there already exists an external one.

Solution:

```Cpp
class A: public enable_shared_from_this<A> {
    void get_shared_ptr() {
        return shared_from_this();
    }
}
```

Note that the base class is templatized on the derived class. This design pattern is called CRTP(The Curiously Recurring Template Pattern).

shared_from_this creates a shared_ptr without duplicating control blocks by looking up the control block for the current object. If no such block exists, the behaviour is undefined. To prevent this, people usually private the ctors, and provides a factory function that directly returns a shared_ptr to an object.

## Use weak_ptr for shared_ptr-like pointers that can dangle

Idea: A smart pointer acting like shared_ptr, but not affecting the reference count.
Problem: The object may be destroyed, and the pointer will dangle.
Solution: track when the object dangles.

### Functionality

```Cpp
auto wPtr = weak_ptr<T>(sPtr); // Creation
wPtr.expired(); // Check whether the object has been destroyed
```

### Dereferencing

```Cpp
*wPtr // Error, no dereferencing or ->
auto sPtr = wPtr.lock(); // returns shared_ptr, or null if expired.
shared_ptr<T> sPtr1(wPtr); // throw bad_weak_ptr if expired
```

### Usage

#### Dereferencing

Dangerous:

```Cpp
// the object may be destroyed between expired() and lock()!s
if (!wp.expired()) {
    wp.lock()->DoSomething();
}
```

Instead, use atomic operation:

```Cpp
auto sp = wp.lock();
if (sp) {
    sp->DoSomething();
}
```

#### Scenes

- Observer design pattern:
Subject: whose state may change
Observers: who should be notified if subject's state changes
Subjects should not control the lifetime of their observers, so they should use weak_ptr to refer to obs.
- Avoid shared_ptr cycle
If two objects point to each other both with shared_ptr, then neither can be released. So, either should use a weak_ptr.

### Performance

Same size and cost (because of using atomic operation) as shared_ptr. Weak_ptr manipulates weak count, see next item.

## Prefer make_unique and make_shared to direct use of new

Idea: take parameters and perfect them to ctors, creating a smart pointer to the object.

### Advantages

1. Less work

```Cpp
auto spw1(make_shared<A>());
shared_ptr<A> spw2(new A);
```

2. Exception Safety

Consider:

```Cpp
processA(shared_ptr<A>(new A), doSomeThing());
```

The following things happen:

- an object of A is allocated on the heap
- a shared pointer is created
- doSomeThing() executes

However, although the second is guaranteed to happen after the first, the third can be in any order, e.g., 

- object created
- doSomeThing() throws an exception!
- shared_ptr should be created, but not - memory leak!

make_shared avoids such a problem.

1. Efficiency (make_shared only)

```Cpp

// Two allocations:
// one for object, the other for control block
shared_ptr<A> spa(new A);
// One allocation: a single trunk to hold both
make_shared<A> spa1(new A);
```

### Where make shouldn't or cannot be used

- make_shared / make_unique doesn't support custom deleter
- perfect forward arguments to ctors problem:

```Cpp
auto up = make_unique<vector<int>>(10, 20);
// Calls which:
// vector<int> {10, 20};
// vector<int>(10, 20); // this one!
```

A consequent problem is, to call the ctor for braced initializer, you have to explicitly use new operator, for it cannot be directly perfect-forward. Another approach is to hold it with an auto.