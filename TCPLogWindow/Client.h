#pragma once
#include "networking.h"
#include "DataBuffer.h"
#include "Request.h"

#include <vector>
#include <memory>

namespace Level
{
	enum Level;
}

class Client
{
public:
	Client();

	void run();

	void listenUserRequest();

private:
	bool send();

	bool sendRequest(Request* request);

	bool connect(const ip_address& serverAddr);

	bool receive(char* dataBuffer, size_t bufferSize, size_t& receivedLength);

	bool stringToAddr(std::string& addrString, ip_address& addr);
private:
	/* the client socket does not need to specify port, 
	thus can use the default constructor */
	tcp_socket mSocket;

	DataBuffer mDataBuffer;

	std::vector<std::unique_ptr<Request>> mRequestQueue;
};