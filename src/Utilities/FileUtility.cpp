#include "pch.h"

#include "FileUtility.h"


std::string FileUtility::ReadFile(const std::filesystem::path& filePath)
{
	std::ifstream inFileStream(filePath);
	if (!inFileStream)
	{
		LOG_ERROR("Failed to read file: {0}", filePath.string().data());
		return "";
	}

	std::stringstream buffer;
	buffer << inFileStream.rdbuf();
	return buffer.str();
}

