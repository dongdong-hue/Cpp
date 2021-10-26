#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <chrono>
using namespace boost::asio;
using boost::system::error_code;
boost::asio::io_service __SERVICE;
 
#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)

#pragma pack (push, 1)
struct TcpMsg
{
	uint32_t fixed; // 固定 8888
	uint32_t msg_len;
	uint16_t msg_type;
	char msg_data[];
};
#pragma pack (pop)

class Client : public boost::enable_shared_from_this<Client>
	, boost::noncopyable 
{
	typedef Client self_type;
public:
	Client();
	void start(const ip::tcp::endpoint& ep);
	void stop();
	void io_work();
public:
	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<Client> ptr_;
 
	bool started();
	void WriteMsg();
private:
	void on_connect(const error_code & err);
	void Rev_msg();
	void on_read(const error_code & err, size_t bytes);
	void StartTime();
	void TimerFun(const boost::system::error_code& error);
	int64_t GetNowTimepoint();
	void SendMsg(const std::string& send_msg, const uint16_t& msg_type);

	void on_write(const error_code & err, std::size_t bytes);
	void do_write();
public:
	std::atomic<bool> connect_suc_ = {false};
private:
	boost::asio::io_service service_;
	boost::shared_ptr<ip::tcp::socket> sock_ = {nullptr};
	boost::shared_ptr<boost::asio::deadline_timer> timer_ = {nullptr};

	enum { max_msg = 1024 };
	char read_buffer_[max_msg];
	std::atomic<bool> started_;
	boost::shared_ptr<TcpMsg> tcp_msg_;
	boost::thread AsyncWrite_;
    boost::thread IOwork_;
	int64_t heart_interval_ = {3};
    int64_t last_timepoint_ = {0};
	uint32_t send_num_ = {0};
};