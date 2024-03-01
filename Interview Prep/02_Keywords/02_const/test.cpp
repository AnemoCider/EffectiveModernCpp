class A {
    public:
        mutable int a = 0;
};

int main() {
    const A a {};
    auto p = &a;
    p->a = 2;

    const auto d = new A();
    auto* const e = new A();
    A const* f = new A();
    
    const auto cp = new A();
}