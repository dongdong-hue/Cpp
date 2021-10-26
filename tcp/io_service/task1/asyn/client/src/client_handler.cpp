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
	Client() : started_(true)
	{}
	void start(const ip::tcp::endpoint& ep)
	{
		std::cout << "start------\n";
		sock_ = boost::make_shared<ip::tcp::socket>(service_);

		sock_->async_connect(ep, MEM_FN1(on_connect, _1));
		IOwork_ = boost::thread(boost::bind(&Client::io_work, shared_from_this()));

		timer_ = boost::make_shared<boost::asio::deadline_timer>(service_);
	}
public:
	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<Client> ptr_;
 
	void stop() 
	{
		if (!started_) return;
		started_ = false;
		if (sock_ != nullptr)
			sock_->close();
	}
	bool started() { return started_; }
	void io_work()
	{
		boost::system::error_code err;
		service_.run(err);
		if (err)
		{
			std::cout << "io_work | run stop error msg: " << err.message() << "\n";
		}
	}
	void WriteMsg()
	{
		std::cout << "start put msg----\n";
		do_write();
	}
private:
	void on_connect(const error_code & err) 
	{
		if (!err)
		{
			std::cout << "connect on_connect!!!!!!\n";     
			connect_suc_ = true;
			AsyncWrite_ = boost::thread(boost::bind(&Client::WriteMsg, shared_from_this()));
			Rev_msg();
			StartTime();
		}   
		else  
		{     
			std::cout << "connect error!!!!!!\n";     
			stop();
		}
	}
	void Rev_msg()
	{
		std::cout << "recive msg start!!!!!\n";
		sock_->async_read_some(buffer(read_buffer_, 1024), MEM_FN2(on_read, _1, _2));		
	}
	void on_read(const error_code & err, size_t bytes) 
	{
		if (!err) 
		{
			std::string rcv_msg(read_buffer_, bytes);
			std::cout <<"message: " << rcv_msg << std::endl;
			Rev_msg();
		}
		else
		{
			std::cout << "read error!!!!!!\n";     
			stop();
		}
	}
	void StartTime()
	{
		if (!connect_suc_)
		{ 
			sleep(heart_interval_);
			stop();
		}
		if (heart_interval_ <= 0) return;
		timer_->expires_from_now(boost::posix_time::milliseconds(heart_interval_ * 1000));
		timer_->async_wait(boost::bind(&Client::TimerFun, shared_from_this(), _1));
	}
	void TimerFun(const boost::system::error_code& error)
	{
		if (error)
		{
			std::cout << "TimerFun | error msg:" << error.message() << std::endl;
			return;
		}
		else
		{
			auto now = GetNowTimepoint();
			if (now - last_timepoint_ >= 3 * heart_interval_ && last_timepoint_ != 0)
			{
				std::cout << "heart beat timeout--------\n";
				return;
			}
			else if (now - last_timepoint_ >= heart_interval_)
			{
				SendMsg(std::string("123456"), 1);
				last_timepoint_ = now;
			}
			StartTime();
		}
	}
	int64_t GetNowTimepoint()
	{
		auto now = std::chrono::system_clock::now();
		return std::chrono::duration_cast<std::chrono::seconds>
			(now.time_since_epoch()).count();
	}
	void SendMsg(const std::string& send_msg, const uint16_t& msg_type)
	{
		std::vector<char> msg_ptr;
		msg_ptr.resize(sizeof(TcpMsg) + send_msg.length());
		TcpMsg* tcp_msg = (TcpMsg*)(msg_ptr.data());
		tcp_msg->fixed = (uint32_t)8888;
		tcp_msg->msg_len = (uint32_t)(send_msg.length());
		tcp_msg->msg_type = (uint16_t)msg_type; 
		memcpy(tcp_msg->msg_data, send_msg.data(), send_msg.length());
		async_write(*sock_, boost::asio::buffer(msg_ptr.data(), msg_ptr.size()),
				MEM_FN2(on_write, _1, _2));		
	}

	void on_write(const error_code & err, std::size_t bytes) 
	{
		if (err)
		{
			std::cout << "send failed: " << err.message() << std::endl;
		}
		else
		{
			++send_num_;
		}
	}
public:
	void do_write() 
	{
		if (!started()) return;
		std::string str;
		std::cout << "输入消息\n";
		getline(std::cin, str);
		std::cout << "要发送的消息为: " << str << "-----\n\n";
		SendMsg(str, 2);
		WriteMsg();
	}
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