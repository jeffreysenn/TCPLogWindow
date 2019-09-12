#pragma once
#include <string>
#include "networking.h"

namespace Utility
{
	inline void printIPAddr(const ip_address& addr)
	{
		std::string OUTaddressString;
		addr.as_string(OUTaddressString);
		printf("%s: \r\n", OUTaddressString.c_str());
	}
}