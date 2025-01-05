#include "pch.h"

#include "Logs.h"


std::string ThisThreadIdToString()
{
	std::ostringstream oss;
	oss << std::this_thread::get_id();
	return oss.str();
}
