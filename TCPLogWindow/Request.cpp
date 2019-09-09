#include "Request.h"
#include "networking.h"

Request::Request()
	: mPut(nullptr)
	, mLevel(Level::Error)
	, mLength(0)
	, mBody(nullptr)
{
}

Request::Request(const std::string& put, Level::Level level, size_t length, const std::string& body)
	: mPut(&put)
	, mLevel(level)
	, mLength(length)
	, mBody(&body)
{
}

void Request::setBody(size_t length, const std::string& body)
{
	mLength = length;
	mBody = &body;
}

std::string Request::formRequestString()
{
	time time = time::now();
	std::string timeString = std::to_string(time.as_milliseconds());

	std::string request =
		"Put " + *mPut + "\r\n"
		+ "Timestamp: " + timeString + "\r\n"
		+ "Level: " + Level::toString(mLevel) + "\r\n"
		+ "Length: " + std::to_string(mLength) + "\r\n"
		+ "\r\n"
		+ *mBody
		+ "\r\n";

	return request;
}
