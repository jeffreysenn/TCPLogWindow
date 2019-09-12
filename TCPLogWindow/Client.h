#pragma once
#include "networking.h"
#include "DataBuffer.h"

class Client
{
public:
	Client();

	void run();

private:
	bool send();

	bool connect();

	bool receive(char* dataBuffer, size_t bufferSize, size_t& receivedLength);
private:
	/* the client socket does not need to specify port, 
	thus can use the default constructor */
	tcp_socket mSocket;

	DataBuffer mDataBuffer;
};