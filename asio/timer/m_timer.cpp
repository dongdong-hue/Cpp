#define BOOST_ASIO_DISABLE_STD_CHRONO
#include<boost/asio.hpp>
#include<boost/date_time/posix_time/posix_time.hpp>
#include<boost/system/error_code.hpp>
// #include <boost/thread.hpp>
// #include <boost/bind.hpp>
// #include <boost/shared_ptr.hpp>
// #include <boost/enable_shared_from_this.hpp>
#include<iostream>
using namespace boost::asio;

class base 
{
    virtual void 
}


int main()
{
    io_service ios;
    while(1)
    {
        deadline_timer t(ios, boost::posix_time::seconds(2));
        
        std::cout << t.expires_at() << std::endl;

        t.wait();
        std::cout << "hello asio" << std::endl;
    }
    return 0;
}