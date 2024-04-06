# Concurrency

## Ordering

### Order of Evaluation

- order of operators' associativity
  - `a() + b() + c()` will be *calculated* from left to right, as per the associativity of `+`
- order of evaluation
  - in `a() + b() + c()`, `a()`, `b()`, `c()` can be *evaluated* in any order

## Example: Single Producer Single Consumer Lock-Free FIFO

### Problem

`mutex` has a large overhead, thus we want to make the concurrent data structure lock-free.

### Building Blocks

- `atomic`
- `Compare And Swap (CAS)`: atomically, if is expected value, set to desired and return true; return false directly otherwise.

### Attempts

#### Write as usual

- push: if full (`back - front == size`) return false , else push_back (`back++`).
- pop: if empty (`back == front`) return false, else pop_front (`front--`).

Problem:

- data race on `back` and `front`, and when a data is being pushed and popped at the same time.

#### Use a mutex

`mutex.lock` is way too slow.

#### Use `atomic`

Consider this code:

```Cpp
void push(int val) {
    if (front - back != size) {
        data[back % size] = val;
        back++;
    }
}
```

Based on C++'s sequentially consistent ordering, atomic operations in this code serve as memory barriers.

### Optimizations

#### False sharing

- `back` and `front` may reside in the same cache line (typically 64 bytes)
- however, they don't need to be updated at the same time
  - Consider: thread1 writes `back`, thread2 reads `front`, finding that the cache is dirty
  - thread2 has to wait for thread1 to write the cache to memory, which is unnecessary

solution, e.g., 

```Cpp
alignas(std::hardware_destructive_interference_size) std::atomic<std::size_t> front;
```

#### More

To be learned later. Refer to *Single Producer Single Consumer Lock-free FIFO From the Ground Up* at CppCon 2023.