#include "Utils.h"


Utils& Utils::getInstance()
{
	static Utils utils;
	return utils;
}

std::vector<char> Utils::readFromBinaryFile(const std::filesystem::path& pPath)
{
	std::ifstream file(pPath, std::ios::binary | std::ios::ate);
	if (!file.is_open())
		throw std::runtime_error("File was not opened!\n");
	
	std::streamsize fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(fileSize);
	if(!file.read(buffer.data(), fileSize))
		throw std::runtime_error("File was not read!\n");

	return buffer;
}
