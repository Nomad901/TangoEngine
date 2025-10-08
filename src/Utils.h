#pragma once
#include <iostream>
#include <format>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

class Utils
{
public:
	static Utils& getInstance();
	
	std::vector<char> readFromBinaryFile(const std::filesystem::path& pPath,
										 int64_t& pSize);
	std::vector<float> readFromBinaryFile2Float(const std::filesystem::path& pPath,
												int64_t& pSize);

	float randomFloatRange(float pStart, float pEnd);
	float randomFloat();
	
	int32_t calcNextPowerOfTwo(int32_t pX);

	Utils(const Utils&) = delete;
	Utils& operator=(const Utils&) noexcept = delete;
	Utils(Utils&&) = delete;
	Utils& operator=(Utils&&) noexcept = delete;

private:
	Utils() = default;
	~Utils() = default;
};

