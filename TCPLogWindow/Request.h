#pragma once
#include <string>
#include <memory>
#include <array>
#include <exception>

namespace Level
{
	enum Level
	{
		Error,
		Warn,
		Info,
		Debug,
		None,
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


struct Request
{
	Request();

	bool operator==(const Request& rhs) const;
	bool operator!=(const Request& rhs) const;

	std::string formRequestStringClient();
	std::string formRequestStringServer() const;

	std::string put;
	float timeStamp;
	Level::Level level;
	size_t bodyLength;
	std::string body;
};