#pragma once
#include <string>

namespace Utility
{
	inline std::string splitStringTillEnd(const std::string& delimiter, const std::string& string)
	{
		size_t delimiterPos = string.find(delimiter);
		if (delimiterPos != std::string::npos)
		{
			size_t afterDelimiterPos = delimiterPos + delimiter.length();
			return string.substr(afterDelimiterPos, string.length() - afterDelimiterPos - 1);
		}
		else
			return std::string("error");
	}
}