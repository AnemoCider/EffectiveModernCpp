#include <iostream>

using namespace std;

class Point {
public:
    int x;
    int y;
    int getX() {return x;}
    Point(int x) : x(x) {};
    constexpr Point() : x(1), y(1) {};
};

int main() {
    Point p1 {};
    cout << p1.x << '\n';
    constexpr Point p2 {};
    cout << p2.x << '\n';
}