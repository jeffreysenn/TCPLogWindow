#include "Request.h"
#include "networking.h"

namespace Level
{
	const Level toLevel(const std::string& string)
	{
		for (auto it = LevelStrings.begin(); it != LevelStrings.end(); ++it)
		{
			if (string == *it)
			{
				size_t index = it - LevelStrings.begin();
				return static_cast<Level>(index);
			}
		}
		return Level::Error;
	}
}

Request::Request()
	: put()
	, timeStamp(0)
	, level(Level::Error)
	, length(0)
	, body()
{
}

Request::Request(const std::string&& put, Level::Level level, size_t length, const std::string&& body)
	: put(put)
	, timeStamp(0)
	, level(level)
	, length(length)
	, body(body)
{
}

bool Request::operator==(const Request& rhs) const
{
	return level == rhs.level
		&& length == rhs.length
		&& timeStamp == rhs.timeStamp
		&& put == put;
}

bool Request::operator!=(const Request& rhs) const
{
	return !(*this==rhs);
}

std::string Request::formRequestStringClient()
{
	timeStamp = time::now().as_milliseconds();

	std::string request =
		"Put " + put + "\r\n"
		+ "Timestamp: " + std::to_string(timeStamp) + "\r\n"
		+ "Level: " + Level::toString(level) + "\r\n"
		+ "Length: " + std::to_string(length) + "\r\n"
		+ "\r\n"
		+ body;

	return request;
}

std::string Request::formRequestStringServer()
{
	std::string request = put + " " + std::to_string(timeStamp) + " "
		+ Level::toString(level) + " " + body	+ "\r\n";

	return request;
}

