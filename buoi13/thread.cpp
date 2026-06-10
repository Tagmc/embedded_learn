#include <thread>
#include <iostream>
#include <vector>
#include <mutex>

std::mutex counterMutex;

void increaseMany(int &counter)
{
    for (int i = 1; i <= 100; ++i)
    {
        std::lock_guard<std::mutex> lock(counterMutex);
        ++counter;
    }
}

int main()
{
    int counter = 0;
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i)
    {
        threads.push_back(std::thread(increaseMany, std::ref(counter)));
    }

    for (std::thread &thread : threads)
    {
        thread.join();
    }
    std::cout << counter << '\n';

    std::cout << "DONE COUNT" << '\n';
    return 0;
}