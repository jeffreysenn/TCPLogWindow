#pragma once
#include "networking.h"
#include "DataBuffer.h"

#include <list>


class Server
{
public:
	Server();

	void run();

private:
	bool openListener();
	bool acceptClient();
	bool receive();
	struct Request reconstructRequest(char* data, size_t size);

private:
	ip_address mAddr;
	tcp_listener mListener;
	std::list<SocketData> mSocketDataList;
};

