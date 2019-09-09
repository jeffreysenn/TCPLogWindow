#include "Server.h"

#include <sstream>

#define TEST_SERVER_ADDRESS 0, 0, 0, 0, 50000
#define DATA_BUFFER_SIZE size_t(1024 * 1024)

Server::Server()
	: mAddr(TEST_SERVER_ADDRESS)
{
}

void Server::run()
{
	printf("Server: \n");

	if (!openListener())
		return;

	while (true)
	{
		if(!(acceptClient()))
			break;

		receive();

	}
}

bool Server::acceptClient()
{
	auto result = mListener.accept();
	if (result.is_success())
	{
		mSocketDataList.push_back(SocketData(result.socket_, result.address_, DATA_BUFFER_SIZE));
	}
	else if (!network::error::is_non_critical(result.code_))
	{
		auto errcode = network::error::get_error();
		auto errmsg = network::error::as_string(errcode);
		printf("listener accepting error is critical - %s (%d) \n", errmsg, errcode);
		return false;
	}
	return true;
}

bool Server::receive()
{
	auto it = mSocketDataList.begin();
	while(it != mSocketDataList.end())
	{
		std::string addressString;
		it->address.as_string(addressString);
		printf("%s: \r\n", addressString.c_str());

		size_t received = 0;
		size_t dataBufferSize = it->dataBuffer.getSize();
		while (received < dataBufferSize)
		{
			auto result = it->socket.receive(
				dataBufferSize - received, 
				(uint8*)it->dataBuffer.getData());

			if (result.is_success())
			{
				received += result.length_;

				char* data = it->dataBuffer.getData();
				// when line count is greater than 5, 
				// header receiving is completed
				if (getLineCount(data) >= 5)
				{
					//read header
					uint32_t lineCount = 0;
					std::istringstream iss;
					std::string line;
					while (getline(iss, line))
					{
						switch (lineCount)
						{
						case 0:

						case 1:
						case 2:
						case 3:
						case 4:
							break;
						};
						lineCount++;
					}
				}

				//it++;
				//break;
			}
			else
			{
				auto errcode = result.code_;
				if(!network::error::is_non_critical(errcode))
				{
					auto errmsg = network::error::as_string(errcode);
					std::string addressString;
					it->address.as_string(addressString);
					printf("receiving error - %s (%d) \n", errmsg, errcode);

					it->socket.close();
					// TODO: remove the socketData from the list
					break;
				}
			}
		}

		it->dataBuffer.getData()[received] = '\0';
		printf(it->dataBuffer.getData());
	}
	return false;
}

uint32_t Server::getLineCount(char* data)
{
	std::istringstream iss(data);
	std::string line;
	uint32_t lineCount = 0;
	while (getline(iss, line))
	{
		++lineCount;
	}
	return lineCount;
}

bool Server::openListener()
{
	if (!mListener.open(mAddr))
	{
		auto errcode = network::error::get_error();
		auto errmsg = network::error::as_string(errcode);
		printf("could not open listener - %s (%d) \n", errmsg, errcode);
		return false;
	}
	return true;
}
