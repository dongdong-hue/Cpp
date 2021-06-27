#ifndef _ASYNC_SERVER_H__
#define _ASYNC_SERVER_H__

// #define BOOST_ASIO_DISABLE_STD_CHRONO
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/function.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <vector>
#include<chrono>

#define HEARTEMSG std::string("123456789")

#pragma pack (push, 1)
struct TcpMsg
{
	uint32_t fixed; // 固定 8888
	uint32_t msg_len;
	uint16_t msg_type;
	char msg_data[];
};
#pragma pack (pop)

using namespace boost::asio;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

class Session : public boost::enable_shared_from_this<Session>
	, boost::noncopyable 
{
public:
    Session();
    ~Session();
    void HadleRead(const boost::system::error_code &err, socket_ptr sock_ptr);
    void handle_accept(const boost::system::error_code &err, socket_ptr sock_ptr);
    void WriteMsg(/*const boost::system::error_code &err, socket_ptr sock_ptr*/);
    void SendMsg(const std::string& send_msg);
    void SendCallFunc(std::size_t msg_len, const boost::system::error_code& err, std::size_t len);
    void Async_write(const boost::system::error_code &err, socket_ptr sock_ptr);
    int64_t GetNowTimepoint();
    void StartTime();
    void TimerFun(const boost::system::error_code& error);
    void start_accept(); 
    void handle_msg(const boost::system::error_code &err, std::size_t rcv_len, char* msg, socket_ptr sock_ptr);
    void io_work();
    void SpliteMsg(char* msg, bool& is_erase, uint32_t& msg_len);
private:
    std::vector<char> read_buffer_;
    char write_buffer_[512];
    boost::asio::io_service service_;
    boost::shared_ptr<boost::asio::ip::tcp::endpoint> ep_ = {nullptr};
    boost::shared_ptr<boost::asio::ip::tcp::acceptor> acc_ = {nullptr};
    socket_ptr sock_ptr_ = {nullptr};
    boost::thread AsyncWrite_;
    boost::thread IOwork_;
    int64_t heart_interval_ = {3};
    int64_t last_timepoint_ = {0};
    boost::asio::deadline_timer timer_{service_};
};

#endif