#pragma once
#include "networking.h"
#include "DataBuffer.h"

namespace Level
{
	enum Level;
}

class Client
{
public:
	Client();

	void run();

private:
	bool send(const std::string& put, Level::Level level, const std::string& body);

	bool connect(const ip_address& serverAddr);

	bool receive(char* dataBuffer, size_t bufferSize, size_t& receivedLength);

	bool stringToAddr(std::string& addrString, ip_address& addr);
private:
	/* the client socket does not need to specify port, 
	thus can use the default constructor */
	tcp_socket mSocket;

	DataBuffer mDataBuffer;
};