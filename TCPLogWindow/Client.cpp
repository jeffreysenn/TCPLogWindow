#include "Client.h"
#include "networking.h"
#include "Request.h"

#include <windows.h>

#define TEST_SERVER_ADDRESS 127, 0, 0, 1, 50000

Client::Client()
{
}

void Client::run()
{
	printf("Client: \n");

	if (!connect())
		return;

	//for(int i = 0; i < 10; ++i)
	while (true)
		send();

	Sleep(1000000);
}

bool Client::send()
{
	Request request;
	request.put = "/game";
	request.level = Level::Info;
	std::string requestBody("A test message \r\n");
	request.length = requestBody.length();
	request.body = std::move(requestBody);

	std::string requestString = request.formRequestStringClient();

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
