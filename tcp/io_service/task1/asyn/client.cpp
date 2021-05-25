#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using boost::system::error_code;
boost::asio::io_service service;
 
#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)

struct TcpMsg
{
	uint32_t msg_len;
	char* msg_data;
}

class Client : public boost::enable_shared_from_this<Client>
	, boost::noncopyable 
{
	typedef Client self_type;
	Client() : sock_(service), started_(true)
	{}
	void start(ip::tcp::endpoint ep)
	{
		sock_.async_connect(ep, MEM_FN1(on_connect, _1));
	}
public:
	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<Client> ptr;
 
	void stop() 
	{
		if (!started_) return;
		started_ = false;
		sock_.close();
	}
	bool started() { return started_; }
private:
	void on_connect(const error_code & err) 
	{
		if (!err)
			connect_suc_ = true;      
		else            
			stop();
	}
	void on_read(const error_code & err, size_t bytes) 
	{
		if (!err) {
			std::string copy(read_buffer_, bytes - 1);
			std::cout << "server echoed our " << message_ << ": "
				<< (copy == message_ ? "OK" : "FAIL") << std::endl;
		}
		// stop();
	}
 
	void on_write(const error_code & err, size_t bytes) 
	{
		do_read();
	}
	void do_read() 
	{
		async_read(sock_, buffer(read_buffer_),
			MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
	}
	void do_write() 
	{
		if (!started()) return;
		std::string str;
		std::cout << "输入消息\n";
		std::cin >> str; 
		std::cout << "要发送的消息为: " << str << "-----\n\n";
		std::shared_ptr<char> msg_ptr = new[sizeof(TcpMsg) + str.size()];
		tcp_msg_ = (TcpMsg*)msg_ptr;
		tcp_msg_->msg_len = str.size();
		memcpy(tcp_msg_->msg_data, str.data(), str.length());
		
		sock_.async_write_some(buffer(write_buffer_, sizeof(TcpMsg) + str.size()),
			MEM_FN2(on_write, _1, _2));
		
	}
	size_t read_complete(const boost::system::error_code & err, size_t bytes) 
	{
		if (err) return 0;
		bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
		return found ? 0 : 1;
	}

	bool connect_suc_ = {false};
private:
	ip::tcp::socket sock_;
	enum { max_msg = 1024 };
	char read_buffer_[max_msg];
	char write_buffer_[max_msg];
	bool started_;
	boost::shared_ptr<TcpMsg> tcp_msg_;
};

int main(int argc, char* argv[]) {
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);
	boost::shared_ptr<Client> client_ptr = new (Client ());
	client_ptr->start();
	while (client_ptr->connect_suc_)
	{
		client_ptr->do_write();
	}

	// std::queue<std::string> msg_queue;
	// for (char ** message = messages; *message; ++message) {
	// 	Client::start(ep, *message);
	// 	boost::this_thread::sleep(boost::posix_time::millisec(100));
	// }
	service.run();
}