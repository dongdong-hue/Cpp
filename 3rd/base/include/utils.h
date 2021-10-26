#ifndef __TRD_UTILS_H_
#define __TRD_UTILS_H_

#include <string>

namespace trd
{

class utils
{
    utils() = delete;
    ~utils() = delete;
public:
    static std::string Hxdstr(const char* buf, std::size_t len);
};

}

#endif