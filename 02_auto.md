# 02 Auto

## Prefer auto to explicit type decl

### Enforced initialization

    int x1; // can compile, unknown value
    auto x2; // definitely won't compile
    auto x3 = 0; // cool

### Deduce weird types

    auto derefLess = [](const auto& p1, const auto& p2) {return *p1 < *p2};

### Work with closure

#### std::function vs function pointer

Func pointer can only point to functions, while std::function can refer to any callable objects, e.g., to store a lambda function.

std::function objects have fixed size. If the size is not adequate to hold the real closure, it will use heap memory. So it would be bigger and slower, than auto-declared, as the example above. std::function would be:

    std::function<bool(const std::unique_ptr<myClass>&, ...)> derefLess = ...;

### Avoid potential mistakes

    unordered_map<string, int> m {};
    for (const pair<string, int>& p : m) {...} // Not accurate!
    for (const pair<const string, int>& p : m) // the key is const!
    for (const auto& p : m) // accurate

### Automatically update

E.g., wrote a function returns int, but later decide to make it return long.

## Use explicit type when auto deduces undesired types

### vector of bool

An example of proxy class.

    vector<bool> b;
    auto val = bool[0]; // is not bool!

Indexing a vector gives reference to element, however, vector bool stores bits, and reference to bits is forbidden! Therefore, it returns vector bool::reference to emulate indexing.

### One solution: cast

    int sz = vec.size(); // implicit conversion
    auto sz = static_cast<int>(vec.size()); // signifies intentional conversion