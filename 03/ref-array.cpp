#include <iostream>

using namespace std;

int main() {
    int* a[5];
    int (*b)[5];
    // int& c[5]; // array of ref, not allowed
    int temp[5];
    int (&d)[5] = temp; // ref to array
}