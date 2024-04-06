#include <string_view>
#include <vector>
#include <iostream>

using namespace std;

int main() {
    vector<string_view> vec {"a", "b", "c"};
    auto n = vec.size();
    for (uint32_t i = 0; i < n; i++) {
        if (vec[i] == "a") {
            cout << i << ' ';
            vec.erase(vec.begin() + i);
            i--;
        }
        
    }
    cout << vec.size() << '\n';
}