// boost库中noncopyable使用
#include <boost/utility.hpp>
#include <boost/format.hpp>
#include <iostream>
using namespace std;
class myclass : public boost::noncopyable {
public:
    myclass() {}
    myclass(int i) {}
};

class Format
{
public:
    Format(){}
    ~Format(){}
    static void Print()
    {
        cout << boost::format("%s: %05d + %d = %d\n") % "sum" % 1 % 2 % (1 +2);
    }
};


int main() {
    // myclass c1();
    // myclass c2(1);
    // myclass c3(c1);
    // myclass c3 = c1;
    // std::cout << "bianyichenggogn" << std::endl;
    Format myformat;
    Format::Print();
    return 0;
}

/*
int main()
{
    char c;
    cout<<"Enter a character:";
    cin>>c;
    cout<<"It is a:";
    switch (c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        cout<<"digiter."<<endl;
        break;
    default:
        cout<<"non-digiter."<<endl;
        break;
    }
    return 0;
}*/
