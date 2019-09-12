#pragma once
#include "DataBuffer.h"
#include "networking.h"

struct SocketData
{
	SocketData(tcp_socket socket, ip_address address, size_t size, time time);
	~SocketData();

	tcp_socket socket;
	ip_address address;
	DataBuffer dataBuffer;
	time lastReceiveTime;
};
