#include "Response.h"

uint8_t Response::statusToCode(Status status)
{
	static const int statusCodeArr[Status::Count] = { 10, 20, 30 };
	return statusCodeArr[status];
}

std::string Response::statusToString(Status status)
{
	static const std::string statusStringArr[Status::Count] = {
		"Successful",
		"Bad Request",
		"Internal Server Error",
	};
	return statusStringArr[status];
}

Response::Response(Status status, float timestamp)
	: status(status)
	, timestamp(timestamp)
{
}

std::string Response::formResponse()
{
	return "LOG " + std::to_string(statusToCode(status)) + "\r\n"
		+ "Timestamp: " + std::to_string(timestamp) + "\r\n"
		+ "Description: " + statusToString(status) + "\r\n";
}

