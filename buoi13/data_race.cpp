#include <thread>
#include <atomic>
#include <iostream>
std::atomic<int> counter = 0;

void increaseCounter()
{
    for (int i = 0; i < 300; ++i)
    {
        ++counter;
    }
}

int main()
{
    std::thread first(increaseCounter);
    std::thread second(increaseCounter);
    first.join();
    second.join();
    std::cout << counter << '\n';
    return 0;
}