#ifndef __SERVER_CONFIG_H__
#define __SERVER_CONFIG_H__

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Sev
{
namespace Config
{

struct Cfg
{
    std::string ip;
    uint16_t port;
};

class ConfigHandle
{
public:
    ConfigHandle(){}
    ~ConfigHandle(){}
    static bool GetCfg(Cfg& cfg, std::string json_path)
    {
        try
        {
            boost::property_tree::ptree pt;
            boost::property_tree::json_parser::read_json(json_path, pt);
            boost::property_tree::ptree sev = pt.get_child("Server");
            cfg.ip = sev.find("server_ip")->second.get_value<std::string>();
            cfg.port = sev.find("server_port")->second.get_value<uint16_t>();
            if (cfg.port == 0)
            {
                std::cout << "config have not server port\n";
                return false;
            }
        }
        catch(...)
        {
            std::cout << "config file: " << json_path << " parser abnormal\n";
            return false;
        }
        return true;
    }
};

}
}
#endif