#include "Utils.h"


Utils& Utils::getInstance()
{
	static Utils utils;
	return utils;
}

std::vector<char> Utils::readFromBinaryFile(const std::filesystem::path& pPath,
										    int64_t& pSize)
{
	std::ifstream file(pPath, std::ios::binary | std::ios::ate);
	if (!file.is_open())
		throw std::runtime_error("File was not opened!\n");
	
	std::streamsize fileSize = file.tellg();
	pSize = fileSize;
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(fileSize);
	if(!file.read(buffer.data(), fileSize))
		throw std::runtime_error("File was not read!\n");

	return buffer;
}

std::vector<float> Utils::readFromBinaryFile2Float(const std::filesystem::path& pPath, int64_t& pSize)
{
	std::ifstream file(pPath, std::ios::binary | std::ios::ate);
	if (!file.is_open())
		throw std::runtime_error("File was not opened!\n");

	std::streamsize fileSize = file.tellg();
	if (fileSize % sizeof(float) != 0)
		throw std::runtime_error("File size is not multiple to float size!\n");

	pSize = fileSize;
	file.seekg(0, std::ios::beg);

	size_t numFloats = fileSize / sizeof(float);
	std::vector<float> buffer(numFloats);
	if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize))
		throw std::runtime_error("Couldnt read data from the file!\n");

	return buffer;
}

std::vector<float> Utils::readFromPNGFile2Float(const std::filesystem::path& pPath, int64_t& pSize)
{
	int32_t width, height, channels;
	uint8_t* imageData = stbi_load(pPath.string().c_str(), &width, &height, &channels, 1);
	if (!imageData)
		throw std::runtime_error("Couldnt load the imageData!\n");
	
	std::vector<float> buffer;
	buffer.reserve(width * height);

	for (int32_t i = 0; i < width * height; ++i)
	{
		float height = imageData[i] / 255.0f;
		buffer.push_back(height);
	}

	return buffer;
}

float Utils::randomFloatRange(float pStart, float pEnd)
{
	if (pStart == pEnd)
		return pStart;

	if (pEnd < pStart)
	{
		std::cout << "Start is bigger than the end!\n";
		return 0.0f;
	}
	
	float delta = pEnd - pStart;
	float randomValue = randomFloat() * delta + pStart;

	return randomValue;
}

float Utils::randomFloat()
{
	float max = RAND_MAX;
	return ((float)rand() / max);
}

int32_t Utils::calcNextPowerOfTwo(int32_t pX)
{
	int32_t ret = 1;
	if (pX == 1)
		return 2;

	while (ret < pX)
	{
		ret = ret * 2;
	}

	return ret;
}

void Utils::getGLVersion(int32_t& pMajor, int32_t& pMinor)
{
	glGetIntegerv(GL_MAJOR_VERSION, &pMajor);
	glGetIntegerv(GL_MINOR_VERSION, &pMinor);
}
