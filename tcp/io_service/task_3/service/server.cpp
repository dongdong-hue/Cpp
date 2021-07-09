#include "server.h"

/***********
 * 1. sudo ifconfig eth0 192.168.0.1 netmask 255.255.255.128
 * 
 * ***********/

Session::Session()
{
    ep_ = boost::make_shared<boost::asio::ip::tcp::endpoint>(ip::address::from_string("192.168.184.128"), 8001); // 监听端口8001
    acc_ = boost::make_shared<boost::asio::ip::tcp::acceptor>(service_, *ep_);
    read_buffer_.clear();
}

Session::~Session()
{}

void Session::HadleRead(const boost::system::error_code &err, socket_ptr sock_ptr) 
{
    char msg[20] = {0};
    memset(msg, 0, 20);
    // std::cout << "msg: " << msg << "=======\n\n";
    // std::cout << "HadleRead---\n\n";
    sock_ptr->async_read_some(buffer(msg, 1), 
        boost::bind(&Session::handle_msg, shared_from_this(), _1, _2, msg, sock_ptr));
}

void Session::handle_accept(const boost::system::error_code &err, socket_ptr sock_ptr) 
{
    if (err) return;
    // 从这里开始, 你可以从socket读取或者写入
    HadleRead(err, sock_ptr);
    Async_write(err, sock_ptr);
    StartTime();
}
void Session::WriteMsg()
{
    char send_buf[512];
    memset(send_buf, 0, 512);
    std::cout << "start write data:\n";
    std::cin >> send_buf;
    std::string send_msg(send_buf);
    SendMsg(send_msg, send_msg.length(), 1);
    boost::system::error_code ec;
    WriteMsg();
}

void Session::SendMsg(std::string& send_msg, const std::size_t msg_len , const uint16_t msg_type)
{
    // memcpy(msg, send_buf, 512);
    boost::shared_array<char> msg_ptr(new char[sizeof(TcpMsg) + msg_len]);
    TcpMsg* tcp_ptr = (TcpMsg*)(msg_ptr.get());
    tcp_ptr->fixed = 8888;
    tcp_ptr->msg_len = msg_len;
    tcp_ptr->msg_type = msg_type;
    memcpy(tcp_ptr->msg_data, send_msg.data(), send_msg.length());

    async_write(*sock_ptr_, boost::asio::buffer(msg_ptr.get(), sizeof(TcpMsg) + msg_len),
            boost::bind(&Session::SendCallFunc, shared_from_this(), sizeof(TcpMsg) + msg_len, _1, _2));
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
            SendMsg(str, str.length(), 2);
            // std::cout << "send a heartbeat------------------------\n\n";
            last_timepoint_ = now;
        }
        StartTime();
    }
}

void Session::start_accept() 
{
    boost::system::error_code ec;
    acc_->open(ep_->protocol(), ec);
    acc_->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true), ec);
    acc_->bind(*ep_, ec);
    acc_->listen(boost::asio::socket_base::max_connections, ec);

    sock_ptr_ = boost::make_shared<ip::tcp::socket>(boost::ref(service_));
    acc_->async_accept(*sock_ptr_, boost::bind(&Session::handle_accept, shared_from_this(),  _1, sock_ptr_));
    // AsyncWrite_(boost::bind(&Session::Async_write, shared_from_this(), _1, sock_ptr_));
    IOwork_ = boost::thread(boost::bind(&Session::io_work, shared_from_this()));
}

void Session::handle_msg(const boost::system::error_code &err, std::size_t rcv_len, char* msg, socket_ptr sock_ptr)
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
        read_buffer_.push_back(msg[0]);
        // std::cout << "handle_msg, recv a msg---\n\n";
        while (read_buffer_.size() > 0)
        {
            if (read_buffer_.size() < sizeof(TcpMsg))
            {
                // std::cout << "rcv length < sizeof(TcpMsg)---\n";
                // HadleRead(ec, sock_ptr);
                break ;
            } 
            TcpMsg* tcp_msg = (TcpMsg*)(read_buffer_.data());
            // std::cout << "tcp_msg->fixed" << tcp_msg->fixed << std::endl;

            if (read_buffer_.size() < tcp_msg->msg_len + sizeof(TcpMsg))
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
            read_buffer_.erase(read_buffer_.begin(), read_buffer_.begin() + tcp_msg->msg_len + sizeof(TcpMsg));
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
    boost::shared_ptr<Session> session_ = boost::make_shared<Session>();
    session_->start_accept();
    while(1);
    return 0;
}
