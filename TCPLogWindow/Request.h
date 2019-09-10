#pragma once
#include <string>
#include <memory>
#include <array>

namespace Level
{
	enum Level
	{
		Error,
		Warn,
		Info,
		Debug,
		Count
	};

	static const std::array<std::string, Level::Count> LevelStrings =
	{
		"Error",
		"Warn",
		"Info",
		"Debug",
	};

	inline const std::string toString(Level level) { return LevelStrings[level]; }
	const Level toLevel(const std::string& string);
}


class Request
{
public:
	Request();
	Request(const std::string&& put, Level::Level level, size_t length, const std::string&& body);

	bool operator==(const Request& rhs) const;
	bool operator!=(const Request& rhs) const;

	std::string formRequestStringClient();
	std::string formRequestStringServer();

	std::string put;
	float timeStamp;
	Level::Level level;
	size_t length;
	std::string body;
};

