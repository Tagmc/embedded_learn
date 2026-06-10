#include <iostream>
#include <cassert>
int main() {
    int a, b;
    std::cin >> a >> b;
    assert(b != 0);
    std::cout << a + b;
}