#include <iostream>
#include <vector>

using namespace std;

class myClass {
public:
    vector<int> data;
    // vector<int> getData() {
    //     return data;
    // }
    // vector<int> getData() & {
    //     return data;
    // }
    vector<int> getData() && {
        return move(data);
    }
};

int main() {
    myClass myC {};
    // myC.getData();
}