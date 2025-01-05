#include "pch.h"

#include "Core.h"


std::string VectorToString(const std::vector<glm::ivec2>& vec)
{
	std::ostringstream oss;
	oss << "[";
	for (size_t i = 0; i < vec.size(); ++i)
	{
		oss << fmt::format("({}, {})", vec[i].x, vec[i].y);
		if (i < vec.size() - 1)
		{
			oss << ", ";
		}
	}
	oss << "]";
	return oss.str();
}
