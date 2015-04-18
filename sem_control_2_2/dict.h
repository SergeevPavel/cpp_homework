#pragma once 
#include <string>
#include <map>

namespace serialization
{
	struct dict 
	{
		std::string					value;
		std::map<std::string, dict>	children;
	};
} // serialization
