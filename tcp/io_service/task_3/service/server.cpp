#include "server.h"
#include <limlog/Log.h>
/***********
 * 1. sudo ifconfig eth0 192.168.0.1 netmask 255.255.255.128
 *
 * ***********/

Session::Session()
{
    // ep_ = boost::make_shared<boost::asio::ip::tcp::endpoint>(ip::address::from_string("192.168.184.128"), 8001); // 监听端口8001
    // acc_ = boost::make_shared<boost::asio::ip::tcp::acceptor>(service_, *ep_);
    read_buffer_.clear();
}

Session::~Session()
{}

void Session::HadleRead(const boost::system::error_code &err, socket_ptr sock_ptr)
{
    // char msg[20] = {0};
    // memset(msg, 0, 20);
    static const uint32_t MaxLen = 8192;
    static boost::shared_array<char> read_buffer(new char[MaxLen]);
    // std::cout << "msg: " << msg << "=======\n\n";
    // std::cout << "HadleRead---\n\n";
    sock_ptr->async_read_some(buffer(read_buffer.get(), MaxLen),
        boost::bind(&Session::handle_msg, shared_from_this(), _1, _2, read_buffer.get(), sock_ptr));
}

void Session::handle_accept(const boost::system::error_code &err, socket_ptr sock_ptr)
{
    if (err) return;
    // 从这里开始, 你可以从socket读取或者写入
    std::cout << "accept a ssesion\n";
    HadleRead(err, sock_ptr);
    Async_write(err, sock_ptr);
    StartTime();
}
void Session::WriteMsg()
{
    std::string str;
    std::cout << "输入消息\n";
    getline(std::cin, str);
    SendMsg(str, str.length(), IOSer::Protol::MsgType::Writemsg);
    WriteMsg();
}

void Session::SendMsg(std::string& send_msg, const std::size_t msg_len , const uint16_t msg_type)
{
    // memcpy(msg, send_buf, 512);
    boost::shared_array<char> msg_ptr(new char[sizeof(IOSer::Protol::TcpMsg) + msg_len]);
    IOSer::Protol::TcpMsg* tcp_ptr = (IOSer::Protol::TcpMsg*)(msg_ptr.get());
    tcp_ptr->fixed = 8888;
    tcp_ptr->msg_len = msg_len;
    tcp_ptr->msg_type = msg_type;
    memcpy(tcp_ptr->msg_data, send_msg.data(), send_msg.length());

    async_write(*sock_ptr_, boost::asio::buffer(msg_ptr.get(), sizeof(IOSer::Protol::TcpMsg) + msg_len),
            boost::bind(&Session::SendCallFunc, shared_from_this(), sizeof(IOSer::Protol::TcpMsg) + msg_len, _1, _2));
}

void Session::SendCallFunc(std::size_t msg_len, const boost::system::error_code& err, std::size_t len)
{
    if (err)
    {
        return;
    }
    if (msg_len != len)
    {
        std::cout << "send msg is not finish\n";
        return;
    }
    // std::cout << "SendCallFunc | send a msg\n";
}

void Session::Async_write(const boost::system::error_code &err, socket_ptr sock_ptr)
{
    if (err) return;
    AsyncWrite_ = boost::thread(boost::bind(&Session::WriteMsg, shared_from_this()));
}

int64_t Session::GetNowTimepoint()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>
        (now.time_since_epoch()).count();
}

void Session::StartTime()
{
    if (heart_interval_ <= 0) return;
    timer_.expires_from_now(boost::posix_time::milliseconds(heart_interval_ * 1000));
    timer_.async_wait(boost::bind(&Session::TimerFun, shared_from_this(), _1));
}

void Session::TimerFun(const boost::system::error_code& error)
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
            std::string str;
                        str += "123456";
            SendMsg(str, str.length(), IOSer::Protol::MsgType::HBmsg);
            // std::cout << "send a heartbeat------------------------\n\n";
            last_timepoint_ = now;
        }
        StartTime();
    }
}
bool Session::Init()
{
    if (!Sev::Config::ConfigHandle::GetCfg(cfg_, "./etc/server.json"))
    {
        std::cout << "Init GetCfg error\n";
        return false;
    }
    std::cout << "cfg_.ip: " << cfg_.ip << "port: " << cfg_.port << std::endl;
    ep_ = boost::make_shared<boost::asio::ip::tcp::endpoint>(ip::address::from_string(cfg_.ip), cfg_.port); // 监听端口8001
    acc_ = boost::make_shared<boost::asio::ip::tcp::acceptor>(service_, *ep_);
    return true;
}

void Session::Start()
{
    if (!Init())
        return ;
    boost::system::error_code ec;
    acc_->open(ep_->protocol(), ec);
    acc_->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true), ec);
    acc_->bind(*ep_, ec);
    acc_->listen(boost::asio::socket_base::max_connections, ec);

    AsyncAccept();
    IOwork_ = boost::thread(boost::bind(&Session::io_work, shared_from_this()));
}

void Session::Stop()
{

}

void Session::AsyncAccept()
{
    std::cout << "AsyncAccept start !!!!\n";
    sock_ptr_ = boost::make_shared<ip::tcp::socket>(boost::ref(service_));
    acc_->async_accept(*sock_ptr_, boost::bind(&Session::handle_accept, shared_from_this(),  _1, sock_ptr_));
}

void Session::handle_msg(const boost::system::error_code &err, std::size_t rcv_len,
                         const char* msg, socket_ptr sock_ptr)
{
    boost::system::error_code ec;
    if (err)
    {
        std::cout << msg << " -----\n";
        std::cout << err.message() << "未接收到有效\n";
        return;
    }
    else
    {
        read_buffer_.append(msg, rcv_len);
        std::cout << "rcv_len: " << rcv_len << " ---\n\n";
        std::cout << "read_buffer_: " << trd::utils::Hxdstr(msg, 1) << std::endl;
        // std::cout << "handle_msg, recv a msg---\n\n";
        while (read_buffer_.size() > 0)
        {
            if (read_buffer_.size() < sizeof(IOSer::Protol::TcpMsg))
            {
                // std::cout << "rcv length < sizeof(TcpMsg)---\n";
                // HadleRead(ec, sock_ptr);
                break ;
            }
            IOSer::Protol::TcpMsg* tcp_msg = (IOSer::Protol::TcpMsg*)(read_buffer_.data());
            // std::cout << "read_buffer_: " << trd::utils::Hxdstr(read_buffer_.data(), read_buffer_.size()) << " ---\n";
            // std::cout << "tcp_msg->fixed" << tcp_msg->fixed << std::endl;

            if (read_buffer_.size() < tcp_msg->msg_len + sizeof(IOSer::Protol::TcpMsg))
            {
                // HadleRead(ec, sock_ptr);
                break;
            }
            std::string msg_data1(tcp_msg->msg_data, tcp_msg->msg_len);
            // std::cout << msg_data1 << std::endl;
            // std::cout << "msg_len: " << tcp_msg->msg_len << " ,msg type: " << tcp_msg->msg_type
            //           << std::endl;
            switch (tcp_msg->msg_type)
            {
                case 2:
                {
                    //心跳
                    // std::cout << "rcv a hb msg, rcv msg: " << tcp_msg->msg_data;
                    break;
                }
                case 1:
                {
                    //正常消息
                    std::cout << "rcv msg is: " << msg_data1 << std::endl;
                    //memset(read_buffer_, 0, 20);
                    break;
                }
                default:
                {
                    std::cout << "un support msg type: " << tcp_msg->msg_type
                              <<  "rcv msg: " << msg_data1 << std::endl;
                    break;
                }
            }
            read_buffer_.erase(read_buffer_.begin(), read_buffer_.begin() + tcp_msg->msg_len + sizeof(IOSer::Protol::TcpMsg));
        }
        HadleRead(ec, sock_ptr);
    }
}

void Session::SpliteMsg(char* msg, bool& is_erase, uint32_t& msg_len)
{

}

void Session::io_work()
{
    boost::system::error_code err;
    service_.run(err);
    std::cout << "io_work | run stop\n";
}

int main()
{
    // {
    //     MyLog mylog;
	//     mylog.Init("./log");// 日志文件存放的路径
    //     for (int i = 0; i < 100; ++i)
    //         mylog.Log("start service!!!");
    // }
    boost::shared_ptr<Session> session_ = boost::make_shared<Session>();
    session_->Start();
    while(1);
    session_->Stop();
    return 0;
}
