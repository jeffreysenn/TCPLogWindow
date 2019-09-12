#include "Server.h"
#include "Request.h"
#include "Utility.h"


#include <array>
#include <exception>

#define TEST_SERVER_ADDRESS 0, 0, 0, 0, 50000
#define DATA_BUFFER_SIZE size_t(1024 * 1024)
#define DISCONNECT_TIME_SECOND 10
#define OUT

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

		// loop through socketData list
		auto it = mSocketDataList.begin();
		while (it != mSocketDataList.end())
		{
			auto socketDataPtr = it->get();

			// Receive data
			Request OUT request;
			Server::Result result;
			result = receive(socketDataPtr);
			// Process immediately if receive unsuccessful
			if(result.type != Result::Success)
				process(it, request, result);
			else
			{
				// Keep reconstructing request and process
				// until reconstruct unsuccessful
				while(true)
				{
					result = reconstruct(socketDataPtr, request);
					process(it, request, result);

					if(result.type != Result::Success)
						break;
				}
			}
		}
	}
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

bool Server::acceptClient()
{
	auto result = mListener.accept();
	if (result.is_success())
	{
		auto socket(std::make_unique<SocketData>(
			result.socket_, result.address_, DATA_BUFFER_SIZE, time::now()));
		mSocketDataList.push_back(std::move(socket));
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

Server::Result Server::receive(SocketData* socketData)
{
	Result result;

	size_t received = 0;
	size_t dataBufferSize = socketData->dataBuffer.getSize();

	while (received < dataBufferSize)
	{
		auto receiveResult = socketData->socket.receive(
			dataBufferSize - received,
			(uint8*)socketData->dataBuffer.getData());

		received += receiveResult.length_;

		// when no data is received
		if (received == 0)
		{
			// return result with timeout error if no data is received for some time
			time noReceiveTime = time::now() - socketData->lastReceiveTime;
			if (noReceiveTime.as_seconds() >= DISCONNECT_TIME_SECOND)
			{
				result.type = Result::Timeout;
				result.errorMsg = "Disconnected client because of time out.\nTime waited: " 
					+ std::to_string(noReceiveTime.as_seconds());
				return result;
			}
			// return nodata result if not timeout
			else
			{
				result.type = Result::NoData;
				return result;
			}
		}
		else if (received >= dataBufferSize)
		{
			result.type = Result::Spamming;
			result.errorMsg = "disconnected client because it is spamming!";
			return result;
		}

		// when no more data is being received
		// update the last receive time and mark result as success
		if (receiveResult.length_ == 0)
		{
			socketData->lastReceiveTime = time::now();
			result.type = Result::Success;
			break;
		}
	}

	return result;
}

Server::Result Server::reconstruct(SocketData* socketData, Request& request)
{
	char* data = socketData->dataBuffer.getData();
	size_t bufferSize = socketData->dataBuffer.getSize();
	size_t OUT headerLength;

	Result result = reconstructHeader(data, bufferSize, request, headerLength);
	if (result.type != Result::Success)
		return result;

	size_t receivedBodyLength = std::strlen(data + headerLength);
	if (receivedBodyLength < request.bodyLength)
	{
		result.type = Result::RequestIncomplete;
		return result;
	}

	request.body = std::move(std::string(data + headerLength, request.bodyLength));

	// We move the data after this request to the beginning
	// of the buffer and set the rest of the buffer to '\0'. 
	size_t requestSize = headerLength + request.bodyLength;
	size_t restSize = bufferSize - requestSize;
	char* requestEnd = data + requestSize;
	memmove(data, requestEnd, restSize);
	memset(data + restSize, '\0', requestSize);

	return result;
}

void Server::process(std::list<std::unique_ptr<SocketData>>::iterator& it, const Request& request, const Result& result)
{
	ip_address addr = it->get()->address;
	switch (result.type)
	{
	case Server::Result::NoData:
	case Server::Result::RequestIncomplete:
		++it;
		break;

	case Server::Result::Timeout:
	case Server::Result::Spamming:
	case Server::Result::BadRequest:
		Utility::printIPAddr(addr);
		printf("Error: %s\n", result.errorMsg.c_str());
		mSocketDataList.erase(it++);
		break;

	case Server::Result::Success:
		Utility::printIPAddr(addr);
		printf("%s\n",request.formRequestStringServer().c_str());
		break;
	}
}

Server::Result Server::reconstructHeader(char* data, size_t size, Request& request, size_t& headerLength)
{
	Result result;

	size_t rnPairCount = 0;
	char* begin = data;
	const char rnPair[] = "\r\n";

	std::array<std::string, 4> headerTokens;

	// find "\r\n"s and increment count
	// move the begin pointer after "\r\n"
	// put the possible tokens into the array
	while (rnPairCount < 5)
	{
		char* pos = strstr(begin, rnPair);
		if (pos != nullptr)
		{
			switch (rnPairCount)
			{
			case 0:
			{
				char* putBegin = begin + sizeof("PUT ") - 1;
				std::string put(putBegin, pos - putBegin);
				headerTokens[0] = std::move(put);
				break;
			}
			case 1:
			{
				char* timeStampBegin = begin + sizeof("Timestamp: ") - 1;
				std::string timeStamp(timeStampBegin, pos - timeStampBegin);
				headerTokens[1] = std::move(timeStamp);
				break;
			}
			case 2:
			{
				char* levelBegin = begin + sizeof("Level: ") - 1;
				std::string level(levelBegin, pos - levelBegin);
				headerTokens[2] = std::move(level);
				break;
			}
			case 3:
			{
				char* lengthBegin = begin + sizeof("Length: ") - 1;
				std::string length(lengthBegin, pos - lengthBegin);
				headerTokens[3] = std::move(length);
				break;
			}
			};

			++rnPairCount;
			begin = pos + sizeof(rnPair) - 1;
		}
		else
			break;
	}

	// We know the header is received
	// completely when "\r\n" count is 5. 
	// Then we start to process it. 
	if (rnPairCount == 5)
	{
		try
		{
			request.put = std::move(headerTokens[0]);
			request.timeStamp = std::stof(headerTokens[1]);
			request.level = Level::toLevel(headerTokens[2]);
			request.bodyLength = static_cast<size_t>(std::stoi(headerTokens[3]));
		}
		catch (std::exception e)
		{
			result.type = Result::BadRequest;
			result.errorMsg = std::move("Bad request \nException: " + std::string(e.what()));
			return result;
		}

		result.type = Result::Success;
		headerLength = begin - data;
		return result;
	}

	// The header is incomplete if rnCount is not 5
	result.type = Result::RequestIncomplete;
	return result;
}

