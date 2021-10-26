#include "client_app.h"


namespace IOSer
{
	namespace ClenitApp
	{
		AsioClient::AsioClient(const std::string& ip, const uint16_t& port)
			: service_ip_(ip), service_port_(port)
		{
			rcv_msg_.clear();
		}
		AsioClient::~AsioClient()
		{}
		void AsioClient::Start()
		{
			boost::asio::ip::tcp::endpoint endpoint_(boost::asio::ip::address::from_string(service_ip_), service_port_);
			io_service_     = boost::make_shared<boost::asio::io_service>();
			io_service_work = boost::make_shared<boost::asio::io_service::work>(*io_service_);
			sock_           = boost::make_shared<boost::asio::ip::tcp::socket>(*io_service_);

			IO_Run_ = boost::thread(boost::bind(&AsioClient::IORun, this));
			boost::system::error_code err;
			sock_->async_connect(endpoint_, boost::bind(&AsioClient::OnAsynCon, this, err));
			timer_ = boost::make_shared<boost::asio::deadline_timer>(*io_service_);
		}
		void AsioClient::Stop()
		{
			std::cout << "start stop!!!\n";
			io_service_work.reset();
			if (IO_Run_.joinable()) IO_Run_.join();
			if (timer_ != nullptr) timer_->cancel();
			if (PushMsgThread_.joinable()) PushMsgThread_.join();
			if (sock_ != nullptr) sock_->close();
			std::cout << "finish stop!!!!\n";
		}
		void AsioClient::IORun()
		{
			boost::system::error_code ec;
			io_service_->run();
			if (ec)
			{
				std::cout << "io_work | run stop error msg: " << ec.message() << "\n";
			}
		}
		void AsioClient::OnAsynCon(const boost::system::error_code & err)
		{
			if (!err)
			{
				std::cout << "connect on_connect!!!!!!\n";
				is_start_ = true;
				PushMsgThread_ = boost::thread(boost::bind(&AsioClient::PushMsg, this));
				OnRead();
				StartTime();
			}
			else
			{
				std::cout << "connect error!!!!!!\n";
				Stop();
			}
		}
		void AsioClient::StartTime()
		{
			if (!is_start_) 
			{
				std::cout << "is not start---\n\n";
				return;
			}
			if (sock_ == nullptr)
			{
				Stop();
				return;
			}
			if (heart_out_ > 3)
			{
				std::cout << "heartbeat timeout\n";
				return;
			}
			heart_out_++;
			WriteHeartMsg();
			timer_->expires_from_now(boost::posix_time::seconds(heart_interval_));
			timer_->async_wait(boost::bind(&AsioClient::StartTime, this));
		}
		void AsioClient::WriteHeartMsg()
		{
			std::string str;
			str += "123456";
			SendMsg(str, str.length(), IOSer::Protol::MsgType::HBmsg);
		}
		void AsioClient::PushMsg()
		{
			if (!is_start_) return;
			std::string str;
			std::cout << "输入消息\n";
			getline(std::cin, str);
			std::cout << "要发送的消息为: " << str << "-----\n\n";
			SendMsg(str, str.length(), IOSer::Protol::MsgType::Writemsg);
			PushMsg();
		}
		void AsioClient::SendMsg(const std::string& send_msg, const std::size_t msg_len, const uint16_t msg_type)
		{
			boost::shared_array<char> msg_ptr(new char[sizeof(IOSer::Protol::TcpMsg) + msg_len]);
			IOSer::Protol::TcpMsg* tcp_ptr = (IOSer::Protol::TcpMsg*)(msg_ptr.get());
			tcp_ptr->fixed = (uint32_t)8888;
			tcp_ptr->msg_len = (uint32_t)msg_len;
			tcp_ptr->msg_type = (uint16_t)msg_type;
			memcpy(tcp_ptr->msg_data, send_msg.data(), send_msg.length());
			static boost::system::error_code err;
			boost::asio::async_write(*sock_, boost::asio::buffer(msg_ptr.get(), sizeof(IOSer::Protol::TcpMsg) + send_msg.length()),
				[this](boost::system::error_code err, size_t bytes) {
					OnWrite(err, bytes);
				});
		}
		void AsioClient::OnWrite(const boost::system::error_code err, size_t bytes)      // 写消息回调
		{
			if (err)
			{
				std::cout << "error msg: " << err.message() << std::endl;
				return;
			}
			std::cout << "OnWrite bytes: " << bytes << std::endl;
		}
		void AsioClient::OnRead()       // 读消息回调
		{
			static const uint32_t MaxLen = 8192;
			//boost::scoped_array<char> read_buffer_;
			//read_buffer_.reset(new char[MaxLen]);
			static boost::shared_array<char> read_buffer(new char[MaxLen]);
			std::cout << "OnRead------\n\n";
			sock_->async_read_some(boost::asio::buffer(read_buffer.get(), MaxLen), 
				boost::bind(&AsioClient::HandleMsg, this, read_buffer, _1, _2));
				// [&,this](const boost::system::error_code err, std::size_t bytes) {
				// 		HandleMsg(read_buffer, err, bytes);
				// });
		}
		bool AsioClient::SpliteMsg(const char* data, const std::size_t len, std::size_t& msg_len)    // 分割消息
		{
			if (len < sizeof(IOSer::Protol::TcpMsg))
				return false;
			IOSer::Protol::TcpMsg* tcp_ptr = (IOSer::Protol::TcpMsg*)(data);
			if (len < tcp_ptr->msg_len + sizeof(IOSer::Protol::TcpMsg))
				return false;
			msg_len = tcp_ptr->msg_len + sizeof(IOSer::Protol::TcpMsg);
			return true;
		}
		void AsioClient::HandleMsg(boost::shared_array<char> read_buffer,
								   const boost::system::error_code& err, std::size_t bytes)    // 处理消息
		{
			if (err || sock_ == nullptr)
			{
				std::cout << "HandleMsg err" << err.message() << std::endl;
				return ;
			}
			rcv_msg_.append(read_buffer.get(), bytes);
			std::size_t msg_len = 0;
			while (SpliteMsg(rcv_msg_.data(), rcv_msg_.length(), msg_len))
			{
				HandleMsg(rcv_msg_.data(), msg_len);
				rcv_msg_.erase(rcv_msg_.begin(), rcv_msg_.begin() + msg_len);
				msg_len = 0;
			}
			rcv_msg_.erase(rcv_msg_.begin(), rcv_msg_.begin() + msg_len);
			OnRead();
		}
		void AsioClient::HandleMsg(const char*  data, const std::size_t msg_len)
		{
			IOSer::Protol::TcpMsg* tcp_ptr = (IOSer::Protol::TcpMsg*)(data);
			if (tcp_ptr->fixed != 8888)
			{
				std::cout << "fixed error: " << tcp_ptr->fixed << std::endl;
				return;
			}
			switch (tcp_ptr->msg_type)
			{
				case IOSer::Protol::MsgType::HBmsg:
				{
					HanleHBMsg(tcp_ptr, msg_len);
					std::cout << "rsv a hb msg\n";
				}
					break;
				case IOSer::Protol::MsgType::Writemsg:
				{
					HanleReadMsg(tcp_ptr, msg_len);
				}
					break;
				default:
				{
					std::cout << "unkown msg type: " << tcp_ptr->msg_type << std::endl;
				}
					break;
			}
			return;
		}
		void AsioClient::HanleHBMsg(IOSer::Protol::TcpMsg* tcp_ptr, const std::size_t msg_len)
		{
			heart_out_ = 0;
		}
		void AsioClient::HanleReadMsg(IOSer::Protol::TcpMsg* tcp_ptr, const std::size_t msg_len)
		{
			std::string msg(tcp_ptr->msg_data, tcp_ptr->msg_len);
			std::cout << "rcv msg: " << msg << std::endl;
		}
	}
}