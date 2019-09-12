#include "Client.h"
#include "Request.h"

#include <windows.h>
#include <iostream>

#define TEST_SERVER_ADDRESS 127, 0, 0, 1, 50000
#define DATA_BUFFER_SIZE 1024 * 1024

Client::Client()
	: mDataBuffer(DATA_BUFFER_SIZE)
{
}

void Client::run()
{
	printf("Client: \n");

	if (!connect())
		return;

	int sentCount = 0;

	while (true)
	{
		size_t receivedLength;
		if (receive(mDataBuffer.getData(), mDataBuffer.getSize(), receivedLength))
		{
			if (receivedLength > 0)
			{
				printf("%s\n", mDataBuffer.getData());
				mDataBuffer.clear();
			}
		}
		else
		{
			return;
		}

		for (sentCount; sentCount < 10;  ++sentCount)
		{
			if (!send())
				return;
			Sleep(10);
		}
	}
}

bool Client::send()
{
	Request request;
	request.put = "/game";
	request.level = Level::Info;
	std::string requestBody("A test message");
	request.bodyLength = requestBody.length();
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

bool Client::receive(char* dataBuffer, size_t bufferSize, size_t& receivedLength)
{
	receivedLength = 0;

	while (receivedLength < bufferSize)
	{
		auto receiveResult = mSocket.receive(
			bufferSize - receivedLength,
			(uint8*)dataBuffer);

		receivedLength += receiveResult.length_;

		if (receiveResult.length_ == 0)
		{
			if (!network::error::is_non_critical(receiveResult.code_))
			{
				auto errcode = network::error::get_error();
				auto errmsg = network::error::as_string(errcode);
				printf("cannot receive from server - %s (%d) \n", errmsg, errcode);
				return false;
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}
