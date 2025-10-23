#pragma once
#include <iostream>
#include <format>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include "dependencies/stb_image.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

class Utils
{
public:
	static Utils& getInstance();

	/*-----FILES-----*/
	std::vector<char> readFromBinaryFile(const std::filesystem::path& pPath,
										 int64_t& pSize);
	std::vector<float> readFromBinaryFile2Float(const std::filesystem::path& pPath,
												int64_t& pSize);
	std::vector<float> readFromPNGFile2Float(const std::filesystem::path& pPath,
											 int64_t& pSize);
	
	/*-----CAMERA-FRUSTUM MATH-----*/
	bool isPointInsideFrustum(const glm::vec3& pPointPos, const glm::mat4& pMatrix, 
							  float pMultiplier);
	void calculateClipPlanes(glm::vec4& pLeft, glm::vec4& pRight, glm::vec4& pTop, 
							 glm::vec4& pBottom, glm::vec4& pNear, glm::vec4& pFar,
					   const glm::mat4& pViewProjMat) const;

	/*-----MATH-----*/
	float randomFloatRange(float pStart, float pEnd);
	float randomFloat();
	int32_t calcNextPowerOfTwo(int32_t pX);

	/*-----OPENGL_HELPER-----*/
	void getGLVersion(int32_t& pMajor, int32_t& pMinor);

private:
	Utils() = default;
	~Utils() = default;
	Utils(const Utils&) = delete;
	Utils& operator=(const Utils&) noexcept = delete;
	Utils(Utils&&) = delete;
	Utils& operator=(Utils&&) noexcept = delete;
};

