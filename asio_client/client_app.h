#ifndef __CLIENT_APP_H
#define __CLIENT_APP_H

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/function.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/array.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_array.hpp>
#include <string>
#include <atomic>
#include "protol.h"

namespace IOSer
{
namespace ClenitApp
{

class AsioClient
{
public:
	AsioClient(const std::string& ip, const uint16_t& port);
	~AsioClient();
	void Start();
	void Stop();
	void IORun();
protected:
	void OnAsynCon(const boost::system::error_code & err);
	void OnWrite(const boost::system::error_code err, size_t bytes);      // 写消息回调
	void OnRead();       // 读消息回调
	void WriteHeartMsg();
	void PushMsg();
	void SendMsg(std::string& send_msg, const std::size_t msg_len, const uint16_t msg_type);
	void StartTime();
	bool SpliteMsg(const char* data,const std::size_t len, std::size_t& msg_len);    // 分割消息
	void HandleMsg(boost::shared_array<char> read_buffer, const boost::system::error_code & err, size_t bytes);    // 处理消息
	void HandleMsg(const char*  data, const std::size_t msg_len);
	void HanleHBMsg(IOSer::Protol::TcpMsg* tcp_ptr, const std::size_t msg_len);
	void HanleReadMsg(IOSer::Protol::TcpMsg* tcp_ptr, const std::size_t msg_len);
	
private:
	boost::shared_ptr<boost::asio::io_service> io_service_ = { nullptr };
	boost::shared_ptr<boost::asio::io_service::work> io_service_work = { nullptr };
	boost::shared_ptr<boost::asio::ip::tcp::socket> sock_ = { nullptr };
	boost::shared_ptr<boost::asio::deadline_timer> timer_ = { nullptr };

	boost::thread IO_Run_;
	boost::thread PushMsgThread_;
	std::atomic<bool> is_start_ = { false };

	std::string service_ip_;
	uint16_t    service_port_ = {0};

	std::string rcv_msg_;
	uint16_t heart_interval_ = { 3 };   //心跳时间
	std::atomic<uint16_t> heart_out_ = { 0 };		//累计心跳次数
};

}
}

#endif