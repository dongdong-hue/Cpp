#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <atomic>
#include <iostream>
using namespace boost::asio;    

class Client{
    typedef boost::shared_ptr<boost::asio::ip::tcp::socket> sock_pt_;
public:
    Client(boost::asio::io_service& io) : sio_(io),
        ep(ip::address::from_string("127.0.0.1"), 6688)
    {
        Init();
    }
    void Init()
    {
        running_ = true;
        boost::thread thrd(boost::bind(&Client::DoJob, this));
    }

    void DoJob()
    {
        std::cout << "DoJob" << std::endl;
        int i = 0;
        while(running_)
        {
            std::cout << "DoJob i: " << ++i << std::endl;
            sleep(1);
        }
    }

    void start()
    {
        sock_pt_ sock(new ip::tcp::socket(sio_));
            sock->async_connect(ep,
                bind(&Client::conn_handler, this, placeholders::error, sock)); 
    }
    void stop()
    {
        running_ = false;
        // if (thread_.joinable())
        // {
        //     thread_.join();
        // }
    }

    void conn_handler(const boost::system::error_code& ec, sock_pt_ sock)
    {
        if (ec)
        {
            return;
        }
        std::cout << "recive from: " << sock->remote_endpoint().address() << std::endl;
        boost::shared_ptr<std::vector<char> > str(new std::vector<char>(100, 0));

        sock->async_read_some(buffer(*str),
                bind(&Client::read_handler, this, placeholders::error, str));
        
        start();
    }
    void read_handler(const boost::system::error_code& ec, 
                        boost::shared_ptr<std::vector<char> > str)
    {
        if(ec)
            return;
        std::cout << &(*str)[0] << std::endl;
    }
private:
    boost::asio::io_service &sio_;
    ip::tcp::endpoint ep;
    boost::thread thrd_;
    std::atomic<bool> running_;
};

int main()
{
    try{
        std::cout << "client start." << std::endl;
        io_service ios;

        Client cl(ios);
        ios.run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}