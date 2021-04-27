#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using boost::system::error_code;
boost::asio::io_service service;

#define MEM_FN(x)       boost::bind(&Client::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&Client::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&Client::x, shared_from_this(),y,z)

class Client : public boost::enable_shared_from_this<talk_to_svr>
{
public:
    Client(boost::asio::io_service &io)
    io_(io)
    {}
    void start(ip::tcp::endpoint ep)
	{
		sock_.async_connect(ep, MEM_FN1(on_connect, _1));
	}
    
private:
    boost::asio::io_service io_;
}

int main()
{

}