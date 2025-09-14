#pragma once
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <unordered_map>

#include "glm.hpp"
#include "glad/glad.h"

class Shader
{
public:
	Shader() = default;
	Shader(const std::filesystem::path& pPathVertex,
		   const std::filesystem::path& pPathFragment);
	~Shader();

	void init(const std::filesystem::path& pPathVertex,
			  const std::filesystem::path& pPathFragment);

	void bind();
	void unbind();

	void setUniform1i(std::string_view pName, int32_t pValue);
	void setUniform1f(std::string_view pName, float pValue);
	void setMatrixUniform4fv(std::string_view pName, const glm::mat4& pValue);
	void setUniform3fv(std::string_view pName, const glm::vec3& pData);

	GLuint getUniformLocation(std::string_view pName);
private:
	uint32_t createShaders(std::string_view pVertexSource, std::string_view pFragmentSource);
	uint32_t compileShaders(GLuint pType, const std::string& pStr);
	std::string parsePath(const std::filesystem::path& pPath);

private:
	uint32_t mShaderID;
	std::filesystem::path mPathVertex, mPathFragment;
	std::unordered_map<std::string, GLuint> mStorageLocations;
};

