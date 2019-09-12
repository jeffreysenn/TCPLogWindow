#pragma once
#include "networking.h"
#include "SocketData.h"
#include "Response.h"

#include <list>
#include <memory>

class Server
{
public:
	Server();

	void run();

private:
	struct Result
	{
		enum Type
		{
			Unknown,
			NoData,
			Timeout,
			Spamming,
			BadRequest,
			RequestIncomplete,
			Success,
		};

		Result();

		Type type;
		std::string errorMsg;
	};

private:
	bool openListener();
	bool acceptClient();

	/* receives data and write to the buffer, 
		returns true when data is received, 
		false when there is no data */
	Result receive(SocketData* socketData);
	Result reconstruct(SocketData* socketData, struct Request& request);
	void process(std::list<std::unique_ptr<SocketData>>::iterator& it, const struct Request& request, const Result& result);
	void response(Response::Status status, float timestamp, tcp_socket& socket);

	Result reconstructHeader(char* data, size_t size, struct Request& request, size_t& headerLength);

private:
	ip_address mAddr;
	tcp_listener mListener;
	std::list<std::unique_ptr<SocketData>> mSocketDataList;
};
