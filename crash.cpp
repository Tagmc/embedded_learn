#include <iostream>

int main() {
    int *data = new int[3];
    data[0] = 1;
    data[1] = 2;
    data[2] = 2;
    data[3] = 5;
    std::cout << data[3] << '\n';
    delete[] data;
    return 0;
}