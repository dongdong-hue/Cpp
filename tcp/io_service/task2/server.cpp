#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
class Server
{
    typedef boost::shared_ptr<boost::asio::ip::tcp::socket> sock_pt_;
public:
    Server(boost::asio::io_service& io) :sio_(io), 
    accep_(sio_, ip::tcp::endpoint(ip::tcp::v4(), 6688))
    {
        start();
    }

    void start()
    {
        sock_pt_ sock(new ip::tcp::socket(sio_));
        accep_.async_accept(*sock, bind(&Server::accept_handle, 
                    this, placeholders::error, sock));
    }

    void accept_handle(const boost::system::error_code& ec, sock_pt_ sock)
    {
        if (ec)
        {
            return;
        }
        std::cout << "client: " << sock->remote_endpoint().address() << std::endl;
        std::string str;
        std::cin >> str;
        sock->async_write_some(buffer(str),
                bind(&Server::write_handler, this, placeholders::error));
        
        start();
    }

    void write_handler(const boost::system::error_code& ec)
    {
        std::cout << "send msg complete." << std::endl;
    }

    ~Server()
    {}
private:
    boost::asio::io_service &sio_;
    boost::asio::ip::tcp::acceptor accep_;
};


int main()
{
    try{
        std::cout << "server start." << std::endl;
        io_service ios;

        Server serv(ios);
        ios.run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}