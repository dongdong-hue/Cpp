#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/function.hpp>

using namespace boost::asio;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
// void handle_accept(socket_ptr sock, const boost::system::error_code &err);
// void start_accept(socket_ptr sock);
// void handle_msg(char* msg, socket_ptr sock_ptr_, const boost::system::error_code &err);
class Session : public boost::enable_shared_from_this<Session>
	, boost::noncopyable 
{
public:
    Session(){
    service_ = boost::make_shared<boost::asio::io_service>();
    ep_ = boost::make_shared<boost::asio::ip::tcp::endpoint>(ip::address::from_string("127.0.0.1"), 8001); // 监听端口8001
    acc_ = boost::make_shared<boost::asio::ip::tcp::acceptor>(*service_, *ep_);
    }
    ~Session(){}
void handle_accept(const boost::system::error_code &err, socket_ptr sock_ptr) 
{
    if (err) return;
    // 从这里开始, 你可以从socket读取或者写入
    //socket_ptr sock_ptr(new ip::tcp::socket(service));
    // char data[512];
    // sock_ptr_->async_read_some(buffer(data, 512));
    // std::cout << data << " ----\n\n";
    // std::cout << " connect suc-----\n\n";
    sock_ptr->async_read_some(buffer(read_buffer_, 20), 
                boost::bind(&Session::handle_msg, shared_from_this(), _1, read_buffer_, sock_ptr));
    // socket_ptr sock_ptr(new ip::tcp::socket(service));
    // memset(read_buffer_, 0, 512);
    // start_accept(sock_ptr_);
}

void start_accept() 
{
    boost::system::error_code ec;
    acc_->open(ep_->protocol(), ec);
    acc_->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true), ec);
    acc_->bind(*ep_, ec);
    acc_->listen(boost::asio::socket_base::max_connections, ec);

    sock_ptr_ = boost::make_shared<ip::tcp::socket>(boost::ref(*service_));
    acc_->async_accept(*sock_ptr_, boost::bind(&Session::handle_accept, shared_from_this(),  _1, sock_ptr_) );
    boost::thread IOwork(boost::bind(&Session::io_work, shared_from_this()));
}

void handle_msg(const boost::system::error_code &err, char* msg, socket_ptr sock_ptr)
{
    if (err)
    {
        std::cout << msg << " -----\n\n";
        std::cout << err.message() << "未接收到有效\n";
        // return;
    }
    else
    {
        std::cout << msg << " ++++++++++++++\n\n"; 
        memset(read_buffer_, 0, 20);
        boost::system::error_code ec;
        handle_accept(ec, sock_ptr); 
    } 
}

void io_work()
{
    boost::system::error_code err;
    service_->run(err);
}
private:
    char read_buffer_[20];
    boost::shared_ptr<boost::asio::io_service> service_ = {nullptr};
    boost::shared_ptr<boost::asio::ip::tcp::endpoint> ep_ = {nullptr};
    boost::shared_ptr<boost::asio::ip::tcp::acceptor> acc_ = {nullptr};
    socket_ptr sock_ptr_ = {nullptr};
};

int main()
{
    boost::shared_ptr<Session> session_ = boost::make_shared<Session>();
    session_->start_accept();
    // boost::thread tcp_ser1(boost::bind(&Session::start_accept, &session_));
    // std::unique_ptr<boost::asio::io_service::work> io_service_work_ 
    //             = std::unique_ptr<boost::asio::io_service::work>(service);
    // boost::thread IOwork(boost::bind(&io_work, &session_));
    while(1);
}
