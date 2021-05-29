#include "server.h"

Session::Session()
{
    ep_ = boost::make_shared<boost::asio::ip::tcp::endpoint>(ip::address::from_string("127.0.0.1"), 8001); // 监听端口8001
    acc_ = boost::make_shared<boost::asio::ip::tcp::acceptor>(service_, *ep_);
}

Session::~Session()
{}

void Session::HadleRead(const boost::system::error_code &err, socket_ptr sock_ptr) 
{
    sock_ptr->async_read_some(buffer(read_buffer_, 20), 
        boost::bind(&Session::handle_msg, shared_from_this(), _1, read_buffer_, sock_ptr));
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
    SendMsg(send_msg);
    boost::system::error_code ec;
    WriteMsg();
}

void Session::SendMsg(const std::string& send_msg)
{
    // memcpy(msg, send_buf, 512);
    async_write(*sock_ptr_, boost::asio::buffer(send_msg, send_msg.length()),
            boost::bind(&Session::SendCallFunc, shared_from_this(), send_msg.length(), _1, _2));
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
    }
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
        if (now - last_timepoint_ >= 3 * heart_interval_)
        {
            std::cout << "heart beat timeout--------\n";
            return;
        }
        else if (now - last_timepoint_ >= heart_interval_)
        {
            SendMsg("1234456");
            std::cout << "send a heartbeat------------------------\n\n";
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

void Session::handle_msg(const boost::system::error_code &err, char* msg, socket_ptr sock_ptr)
{
    if (err)
    {
        std::cout << msg << " -----\n\n";
        std::cout << err.message() << "未接收到有效\n";
        return;
    }
    else
    {
        std::cout << msg << "\n";
        memset(read_buffer_, 0, 20);
        boost::system::error_code ec;
        HadleRead(ec, sock_ptr);
    }
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
