#include<memory>
#include<vector>
#include<iostream>
class Child;

class Base
{
public:
    static std::shared_ptr<Base> CreatClass();
    Base(){
        std::cout << "base-----\n\n";
    }
    virtual ~Base(){
        std::cout << "~base-----\n\n";
    }
    virtual void Foo()
    {
        std::cout << "base foo-----\n\n";
    }
};

class Child : public Base
{
public:
    Child(){
        std::cout << "child-----\n\n";
    }
    ~Child()
    {
        std::cout << "~child-----\n\n";
    }
    // void Foo()
    // {
    //     std::cout << "Child foo-----\n\n";
    // }
};

std::shared_ptr<Base> Base::CreatClass()
{
    std::cout << "CreatClass-----\n\n";
    std::shared_ptr<Child> ptr = std::make_shared<Child>();
    std::cout << "11111111-----\n";
    return ptr;
}

int main()
{
    std::shared_ptr<Base> base_ptr = Base::CreatClass();
    std::cout << "CreatClass end-----\n\n";
    base_ptr->Foo();
    return 0;
}