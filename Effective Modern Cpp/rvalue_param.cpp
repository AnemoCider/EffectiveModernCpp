#include <utility>
#include <iostream>

using std::cout;

class Data {
public:
    Data() = default;
    Data(const Data& data) {
        cout << "copy ctor\n";
    }
    Data(Data&& data) {
        cout << "move ctor\n";
    }
    Data operator=(const Data& rhs) {
        cout << "copy assignment\n";
        return Data{};
    }
    Data operator=(Data&& rhs) {
        cout << "move assignment\n";
        return Data{};
    }
};

void test1(const Data& data) {};
void test2(Data&& data) {
    cout << &data << '\n';
};

int main() {
    test1(Data{});
    test2(Data{});
    Data&& data1 = Data{};
    test1(data1);
}