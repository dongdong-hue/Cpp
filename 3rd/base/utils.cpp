#include "include/utils.h"
namespace trd
{

std::string utils::Hxdstr(const char* buf, std::size_t len)
{
    static char co[18] = {'0', '1', '2', '3', '4', '5', '6', 
                            '7', '8', '9', 'A', 'B', 'C', 'D', 
                            'E', 'F', ' '};
    std::string str = "";
    for (std::size_t i = 0; i < len; ++i)
    {
        str.append(1,co[(buf[i] & 0xF0) >> 4]);
        str.append(1,co[buf[i] & 0x0F]);
        str += " ";
    }
    return str;
}

}