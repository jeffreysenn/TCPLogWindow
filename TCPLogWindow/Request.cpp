#include "Request.h"
#include "networking.h"

#include <exception>

namespace Level
{
	static const std::array<std::string, Level::Count> LevelStrings =
	{
		"Error",
		"Warn",
		"Info",
		"Debug",
	};

	const std::string toString(Level level)
	{
		return LevelStrings[level];
	}
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
	, timestamp(0)
	, level(Level::None)
	, bodyLength(0)
	, body()
{
}

Request::Request(const std::string& put, Level::Level level, size_t bodyLength, const std::string& body)
	: put(put)
	, timestamp(0)
	, level(level)
	, bodyLength(bodyLength)
	, body(body)
{
}

std::string Request::formRequestStringClient()
{
	timestamp = time::now().as_milliseconds();

	std::string request =
		"Put " + put + "\r\n"
		+ "Timestamp: " + std::to_string(timestamp) + "\r\n"
		+ "Level: " + Level::toString(level) + "\r\n"
		+ "Length: " + std::to_string(bodyLength) + "\r\n"
		+ "\r\n"
		+ body;

	return request;
}

std::string Request::formRequestStringServer() const
{
	std::string request = put + " " + std::to_string(timestamp) + " "
		+ Level::toString(level) + " " + body;

	return request;
}
