/**********************************************
 *  基于boost asio开发的客户端
 *  异步模式
 *  时间2021.07.02
 *  pengxd
 **********************************************/

#include "client_app.h"
#include <unistd.h>

int main()
{
	IOSer::ClenitApp::AsioClient client("192.168.198.142", 8001);
	client.Start();
	while (1)
	{
		sleep(1);
	}
	client.Stop();
	return 0;
}