#pragma once
#include <iostream>
#include <format>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include "dependencies/stb_image.h"
#include "glad/glad.h"

class Utils
{
public:
	static Utils& getInstance();
	
	std::vector<char> readFromBinaryFile(const std::filesystem::path& pPath,
										 int64_t& pSize);
	std::vector<float> readFromBinaryFile2Float(const std::filesystem::path& pPath,
												int64_t& pSize);
	std::vector<float> readFromPNGFile2Float(const std::filesystem::path& pPath,
											 int64_t& pSize);

	float randomFloatRange(float pStart, float pEnd);
	float randomFloat();
	
	int32_t calcNextPowerOfTwo(int32_t pX);

	void getGLVersion(int32_t& pMajor, int32_t& pMinor);

	Utils(const Utils&) = delete;
	Utils& operator=(const Utils&) noexcept = delete;
	Utils(Utils&&) = delete;
	Utils& operator=(Utils&&) noexcept = delete;

private:
	Utils() = default;
	~Utils() = default;
};

