#include "Client.h"
#include "networking.h"
#include "Request.h"

#define TEST_SERVER_ADDRESS 127, 0, 0, 1, 50000

Client::Client()
{
}

void Client::run()
{
	printf("Client: \n");

	if (!connect())
		return;

	while(true)
		send();
}

bool Client::send()
{
	Request request;
	request.setPut("/game");
	request.setLevel(Level::Debug);
	std::string requestBody("A test message \n");
	request.setBody(requestBody.length(), requestBody);

	std::string requestString = request.formRequestString();

	size_t length = requestString.length();
	size_t sent = 0;
	while (sent < length)
	{
		auto result = mSocket.send(requestString.length() - sent, (uint8*)requestString.c_str() + sent);
		if (result.is_success())
		{
			sent += result.length_;
		}
		else
		{
			auto errcode = result.code_;
			if(!network::error::is_non_critical(errcode))
			{
				auto errmsg = network::error::as_string(errcode);
				printf("failed to send - %s (%d) \n", errmsg, errcode);
				return false;
			}
		}
	}
	return true;
}

bool Client::connect()
{
	ip_address serverAddr(TEST_SERVER_ADDRESS);
	auto result = mSocket.connect(serverAddr);
	if (result.is_failure())
	{
		auto errcode = result.code_;
		auto errmsg = network::error::as_string(errcode);
		printf("failed to connect to server - %s (%d) \n", errmsg, errcode);
		return false;
	}
	return true;
}
