#pragma once
#include "networking.h"
#include <memory>

struct DataBuffer
{
public:
	DataBuffer(size_t size)
		: size(size)
		, data(new char[size]())
	{}

	char* getData() { return data.get(); }
	size_t getSize() const { return size; }

private:
	std::unique_ptr<char> data;
	size_t size;
	
};

struct SocketData
{
	SocketData(tcp_socket socket, ip_address address, size_t size)
		: socket(socket) 
		, address(address)
		, dataBuffer(size)
	{}

	tcp_socket socket;
	ip_address address;
	DataBuffer dataBuffer;
};