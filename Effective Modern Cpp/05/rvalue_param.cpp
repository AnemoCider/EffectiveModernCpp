#include <utility>
#include <iostream>

using namespace std;

class Data {
public:
    Data (const int i) {
        cout << "default\n";
    }
    Data(const Data& rhs) {
        cout << "copy\n";
    } 
    Data(Data&& rhs) {
        cout << "move\n";
    }
    Data& operator=(const Data& rhs) {
        cout << "copy =\n";
        return *this;
    }
    Data& operator=(Data&& rhs) {
        cout << "move =\n";
        return *this;
    }
};



template<typename T>
decltype(auto) myMove(T&& param) {
    using ReturnType = remove_reference_t<T>&&;
    return static_cast<ReturnType>(param);
}

void test(int&& param) {

};

void left(int a) {}
void leftRef(int& a){}
void constLeft(const int a){}
void constLeftRef(const int& a){}

int main() {
    myMove(Data(1));

    int a = 1;
    int&b = a;
    const int&c = a;
    test(1);
    // test(a);
    // test(b);
    // test(c);

    int&& d = move(a);
    left(d);
    leftRef(d);
    constLeft(d);
    constLeftRef(d);
}