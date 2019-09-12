#include "Request.h"
#include "networking.h"

#include <exception>

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
		throw std::exception("Cannot cast to level");
		return Level::None;
	}
}

Request::Request()
	: put()
	, timeStamp(0)
	, level(Level::None)
	, bodyLength(0)
	, body()
{
}

bool Request::operator==(const Request& rhs) const
{
	return level == rhs.level
		&& bodyLength == rhs.bodyLength
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
		+ "Length: " + std::to_string(bodyLength) + "\r\n"
		+ "\r\n"
		+ body;

	return request;
}

std::string Request::formRequestStringServer() const
{
	std::string request = put + " " + std::to_string(timeStamp) + " "
		+ Level::toString(level) + " " + body;

	return request;
}
