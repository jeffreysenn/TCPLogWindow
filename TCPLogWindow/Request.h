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

	const std::string toString(Level level);
	const Level toLevel(const std::string& string);
}


struct Request
{
	Request();
	Request(const std::string& put, Level::Level level, size_t bodyLength, const std::string& body);

	std::string formRequestStringClient();
	std::string formRequestStringServer() const;

	std::string put;
	float timestamp;
	Level::Level level;
	size_t bodyLength;
	std::string body;
};