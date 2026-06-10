#include <iostream>
#include <stdexcept>
#include <vector>

class Resource
{
public:
    Resource()
    {
        std::cout << "Acquire Resource\n";
    }
    ~Resource()
    {
        std::cout << "Release Resource\n";
    }
};

void process()
{
    Resource resource;
    throw std::runtime_error("failed");
}

int main()
{
    try
    {
        /* code */
        process();
    }
    catch(const std::exception& e)
    {
        std::cout << "Caught: " << e.what() << '\n';
    }
}