#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <iostream>
// class test
// {

// };

boost::thread thr_;
boost::thread thr2_;
int i = 0; 
void sub()
{
    ++i;
    std::cout << "sub i: " << i << std::endl;
}

void unsub()
{
    --i;
    std::cout << "unsub i: " << i << std::endl;
}

int main()
{
    thr_(bind())
    while(1);
    return 0;
}