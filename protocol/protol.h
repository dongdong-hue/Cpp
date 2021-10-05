#ifndef __PROTOL__H
#define __PROTOL__H

#include <stdint.h>

namespace IOSer
{
namespace Protol {

#pragma pack (push, 1)

struct TcpMsg
{
	uint32_t fixed; // 固定 8888
	uint32_t msg_len;
	uint16_t msg_type;
	char msg_data[];
};

namespace MsgType
{
	static const uint16_t Writemsg = 1;
	static const uint16_t HBmsg = 2;
	static const uint16_t GetFilename = 3;
}

#pragma pack (pop)
}
}
#endif //