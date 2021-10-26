#include <vector>
#include <iostream>
#include <string.h>

#pragma pack (push, 1)
struct TcpMsg
{
	uint32_t msg_len;
	uint16_t msg_type;
	char msg_data[];
};
#pragma pack (pop)

int main()
{
    std::string msg("123456");
    std::vector<char> msg_ptr;
    msg_ptr.resize(sizeof(TcpMsg) + msg.length() + 1);    
    TcpMsg* tcp_msg = (TcpMsg*)(msg_ptr.data());
    tcp_msg->msg_len = sizeof(TcpMsg) + msg.length() + 1;
    tcp_msg->msg_type = (uint16_t)1;
    memcpy(tcp_msg->msg_data, msg.data(), msg.length());
    std::vector<char> msg2;
    msg2.resize(msg_ptr.size());
    memcpy(msg2.data(), msg_ptr.data(), msg2.size());
    TcpMsg* tcp_msg2 = (TcpMsg*)(msg2.data());
    std::cout << "msg len: " << tcp_msg2->msg_len
              << " ,msg type: " << tcp_msg2->msg_type 
              << " ,msg: " << tcp_msg2->msg_data << std::endl;
    return 0; 
}