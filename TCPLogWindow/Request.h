#pragma once
#include <string>
#include <memory>

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

	static const std::string LevelStrings[Level::Count] =
	{
		"Error",
		"Warn",
		"Info",
		"Debug",
	};

	inline const std::string toString(Level level) { return LevelStrings[level]; }
}


class Request
{
public:
	Request();
	Request(const std::string& put, Level::Level level, size_t length, const std::string& body);

	void setPut(const std::string& put) { mPut = &put; }
	void setLevel(const Level::Level level) { mLevel = level; }
	void setBody(size_t length, const std::string& body);

	std::string formRequestString();

private:
	const std::string* mPut;
	Level::Level mLevel;
	size_t mLength;
	const std::string* mBody;
};

