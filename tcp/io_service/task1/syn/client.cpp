#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
using namespace boost::asio;
using boost::system::error_code;
boost::asio::io_service service;
std::atomic<bool> runing_ = {false};
ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);

size_t read_complete(char * buf, const error_code & err, size_t bytes)
{
    if ( err) return 0;
    bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
    // we read one-by-one until we get to enter, no buffering
    return found ? 0 : 1;
}

void connectpri(error_code & err)
{
    std::cout << "connect succsuss!!!!!!\n";
}

void sync_echo() 
{
    // msg += "\n";
    ip::tcp::socket sock(service);
    // static bool is_first = true;
    // if (is_first)
    // {
        sock.connect(ep);
        std::cout << "connect succsuss!!!!!!\n";
    //     is_first = false;
    // }
    while(1)
    {
        std::string str;
        std::cin >> str;
        str += "\n";
        boost::system::error_code ec;
        sock.write_some(buffer(str), ec);
        if (ec)
        {
            std::cout << ec.message() << "\n";
        }
        std::cout << "write: " << str;
        boost::this_thread::sleep( boost::posix_time::millisec(100));
    }
    // sock.write_some(buffer(msg));
    // char buf[1024];
    //int bytes = read(sock, buffer(buf), boost::bind(read_complete,buf,_1,_2));
    // std::string copy(buf, bytes - 1);
    // msg = msg.substr(0, msg.size() - 1);
    // std::cout << "server echoed our " << msg << ": "
    //           << (copy == msg ? "OK" : "FAIL") << std::endl;
    // sock.close();
}
int main(int argc, char* argv[]) 
{
    char* messages[] = { "John says hi", "so does James",
    "Lucy just got home", "Boost.Asio is Fun!", 0};
    boost::thread_group threads;
    // sync_echo();
    // for ( char ** message = messages; *message; ++message) 
    // while(1)
    // {
    //     std::string str;
    //     std::cin >> str;
        threads.create_thread( boost::bind(sync_echo));
    //     boost::this_thread::sleep( boost::posix_time::millisec(100));
    // }
    service.run();
    threads.join_all();
}