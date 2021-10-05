// boost库中noncopyable使用
#include <boost/utility.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
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

static std::string Hxdstr(char* buf, std::size_t len)
{
    static char co[18] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', ' '};
    std::string str = "";
    for (std::size_t i = 0; i < len; ++i)
    {
        str.append(1,co[(buf[i] & 0xF0) >> 4]);
        str.append(1,co[buf[i] & 0x0F]);
        str += " ";
    }
    return str;
}

namespace bf = boost::filesystem;

int main() {
    // myclass c1();
    // myclass c2(1);
    // myclass c3(c1);
    // myclass c3 = c1;
    // std::cout << "bianyichenggogn" << std::endl;
    // Format myformat;
    // Format::Print();
    std::cout << "start ----\n\n";
    //int i = 0x12340000;
    //std::cout << Hxdstr((char*)&i, sizeof(i)) << std::endl;
    bf::path local_path = bf::current_path();
    bf::path file_path = local_path / "stage/1.txt";
    if (bf::exists(file_path))
    {
        std::ofstream ofs(file_path.string().c_str());
        if (!ofs)
        {
            std::cout << "open file failed\n";
            return 1;
        }
        ofs << "文件函数ofstream\n";
        ofs.flush();
    }
    else
    {
        std::cout << file_path << "文件不存在\n";
        bf::create_directory(file_path);
        file_path
    }
    return 0;
}
