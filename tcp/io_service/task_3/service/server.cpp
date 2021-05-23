#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;

char read_buffer_[512];

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
boost::asio::io_service service;
boost::asio::ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001); // 监听端口8001
boost::asio::ip::tcp::acceptor acc(service, ep);
socket_ptr sock_ptr(new ip::tcp::socket(service));

void handle_accept(socket_ptr sock, const boost::system::error_code &err);
void start_accept(socket_ptr sock);
void handle_msg(char* msg, const boost::system::error_code &err);

void handle_accept(socket_ptr sock_ptr_, const boost::system::error_code &err) 
{
    if (err) return;
    // 从这里开始, 你可以从socket读取或者写入
    //socket_ptr sock_ptr(new ip::tcp::socket(service));
    // char data[512];
    // sock_ptr_->async_read_some(buffer(data, 512));
    // std::cout << data << " ----\n\n";
    std::cout << " connect suc-----\n\n";
    async_read(*sock_ptr_, buffer(read_buffer_, 512), 
                boost::bind(handle_msg, read_buffer_, _1));
    socket_ptr sock_ptr(new ip::tcp::socket(service));
    start_accept(sock_ptr);
}

void start_accept(socket_ptr sock_ptr_) 
{
    // memset(read_buffer_, 0, sizeof(read_buffer_));
    acc.async_accept(*sock_ptr_, boost::bind(handle_accept, sock_ptr_,  _1) );
}

void handle_msg(char* msg, const boost::system::error_code &err)
{
    if (err)
    {
        std::cout << msg << " -----\n\n";
        std::cout << err.message() << "未接收到有效\n";
        return;
    }   
}

int main()
{
    start_accept(sock_ptr);
    service.run();
}
