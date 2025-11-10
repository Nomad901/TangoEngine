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
#include "assimp/Importer.hpp"

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

	/*-----OPENGL_HELPERS-----*/
	void getGLVersion(int32_t& pMajor, int32_t& pMinor);

	/*-----PROGRAM_HELPERS-----*/
	void updateDeltaTime(float pDeltaTime);
	float getDeltaTime();

	/*-----BUFFER_HELPERS-----*/
	bool bufferIsBound(GLenum pTarget, uint32_t pID);
	bool shaderIsBound(uint32_t pID);
	bool stencilBufferIsBound();

	/*-----FILES_HELPERS-----*/
	std::string getDirectoryFromFilePath(const std::filesystem::path& pPath);
	
	/*-----ASSIMP_HELPERS-----*/
	glm::mat4 getGlmMatrix4FromAiMat4x4(const aiMatrix4x4& pAiMatrix4x4);
	aiMatrix4x4 getAiMatrix4x4FromGlmMatrix4(const glm::mat4& pGlmMatrix4);

private:
	Utils() = default;
	~Utils() = default;
	Utils(const Utils&) = delete;
	Utils& operator=(const Utils&) noexcept = delete;
	Utils(Utils&&) = delete;
	Utils& operator=(Utils&&) noexcept = delete;

private:
	float mDeltaTime{ 1.0f };
};

