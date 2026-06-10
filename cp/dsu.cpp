#include <string>
#include <iostream>
class Animal
{
public:
    explicit Animal(std::string name) : name_(std::move(name))
    {
    }
    void eat() const
    {
        std::cout << name_ << " is eating\n";
    }

protected:
    std::string name_;
};

class Dog : public Animal
{
public:
    explicit Dog(std::string name) : Animal(std::move(name)) {}
    void bark() const
    {
        std::cout << name_ << " is barking\n";
    }
};

int main()
{
    Dog dog1("Milu");
    dog1.eat();
    dog1.bark();
    return 0;
}