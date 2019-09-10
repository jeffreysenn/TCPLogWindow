#include "Server.h"
#include "Request.h"
#include "Utility.h"

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
		if (!(acceptClient()))
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
	while (it != mSocketDataList.end())
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

			received += result.length_;

			if (received >= dataBufferSize)
			{
				printf("disconnected client because it is spamming!");
				it->socket.close();
				mSocketDataList.erase(it++);
				break;
			}

			// process data
			char* data = it->dataBuffer.getData();
			Request request = reconstructRequest(data, dataBufferSize);
			if (request != Request())
			{
				printf(request.formRequestStringServer().c_str());

				// TODO: Response to the client

				memset(data, '\0', dataBufferSize);
				++it;
				break;
			}


			//else
			//{
			//	auto errcode = result.code_;
			//	if(!network::error::is_non_critical(errcode))
			//	{
			//		auto errmsg = network::error::as_string(errcode);
			//		std::string addressString;
			//		it->address.as_string(addressString);
			//		printf("receiving error - %s (%d) \n", errmsg, errcode);

			//		it->socket.close();
			//		// TODO: remove the socketData from the list
			//		break;
			//	}
			//}
		}
	}
	return false;
}

Request Server::reconstructRequest(char* data, size_t size)
{
	size_t rnPairCount = 0;
	char* begin = data;
	const char rnPair[] = "\r\n";
	char* pos = nullptr;

	Request request;
	// find "\r\n"s and increment count
	while (rnPairCount < 5)
	{
		char* find = strstr(begin, rnPair);
		if (find != nullptr)
		{
			pos = find;
			switch (rnPairCount)
			{
			case 0:
			{
				char* putBegin = begin + sizeof("PUT ") - 1;
				std::string put(putBegin, pos - putBegin);
				request.put = std::move(put);
				break;
			}
			case 1:
			{
				char* timeStampBegin = begin + sizeof("Timestamp: ") - 1;
				std::string timeStamp(timeStampBegin, pos - timeStampBegin);
				request.timeStamp = std::stof(timeStamp);
				break;
			}
			case 2:
			{
				char* levelBegin = begin + sizeof("Level: ") - 1;
				std::string level(levelBegin, pos - levelBegin);
				request.level = Level::toLevel(level);
				break;
			}
			case 3:
			{
				char* lengthBegin = begin + sizeof("Length: ") - 1;
				std::string length(lengthBegin, pos - lengthBegin);
				request.length = static_cast<size_t>(std::stoi(length));
				break;
			}
			};

			++rnPairCount;
			begin = pos + sizeof(rnPair) - 1;
		}
		else return Request();
	}

	// We know the header is received and processed
	// completely when "\r\n" count is 5. 
	// Then we start to process the body. 
	if (rnPairCount == 5)
	{
		size_t receivedBodyLength = std::strlen(begin);
		// check if the body is intact
		if (receivedBodyLength >= request.length)
		{
			std::string body(begin, request.length);
			request.body = std::move(body);

			// Now we have read and reconstructed the request. 
			return request;
		}
	}

	return Request();
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
