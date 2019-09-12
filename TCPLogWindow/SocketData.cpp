#include "SocketData.h"

SocketData::SocketData(tcp_socket socket, ip_address address, size_t size, time time)
	: socket(socket)
	, address(address)
	, dataBuffer(size)
	, lastReceiveTime(time)
{
}

SocketData::~SocketData()
{
	socket.close();
}