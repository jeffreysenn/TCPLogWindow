#pragma once
#include "networking.h"

class Client
{
public:
	Client();

	void run();

private:
	bool send();

	bool connect();

private:
	/* the client socket does not need to specify port, 
	thus can use the default constructor */
	tcp_socket mSocket;
};