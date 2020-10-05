#include <iostream>

using namespace std;
char key[256]=" ";
int main()
{
    if(key==" ")
    cout<<key<<endl;
    else
    cout<<"key isn't NULL"<<endl;
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
