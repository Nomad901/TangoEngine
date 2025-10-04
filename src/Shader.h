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
	Shader(const std::filesystem::path& pPathVertex,
		   const std::filesystem::path& pPathFragment,
		   const std::filesystem::path& pPathGeometry);
	Shader(const std::filesystem::path& pPathVertex,
		   const std::filesystem::path& pPathFragment,
		   const std::filesystem::path& pPathTCS,
		   const std::filesystem::path& pPathTES);
	~Shader();
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) noexcept = delete;
	Shader(Shader&&) = default;
	Shader& operator=(Shader&&) noexcept = default;

	void init(const std::filesystem::path& pPathVertex,
			  const std::filesystem::path& pPathFragment);
	void init(const std::filesystem::path& pPathVertex,
			  const std::filesystem::path& pPathFragment, 
			  const std::filesystem::path& pPathGeometry);
	void init(const std::filesystem::path& pPathVertex,
		 	  const std::filesystem::path& pPathFragment,
			  const std::filesystem::path& pPathTCS,
			  const std::filesystem::path& pPathTES);

	void bind();
	void unbind();

	uint32_t getID() const noexcept;

	void setUniform1i(std::string_view pName, int32_t pValue);
	void setUniform1f(std::string_view pName, float pValue);
	void setMatrixUniform4fv(std::string_view pName, const glm::mat4& pValue);
	void setUniform3fv(std::string_view pName, const glm::vec3& pData);
	void setUniform4fv(std::string_view pName, const glm::vec4& pData);
	void setUniform2fv(std::string_view pName, const glm::vec2& pData);

	std::string getResourcePath() const noexcept;

	GLuint getUniformLocation(std::string_view pName);
private:
	uint32_t createShaders(std::string_view pVertexSource, std::string_view pFragmentSource);
	uint32_t createShaders(std::string_view pVertexSource, std::string_view pFragmentSource, std::string_view pGeometrySource);
	uint32_t createShaders(std::string_view pVertexSource, std::string_view pFragmentSource, 
						   std::string_view pTCSSource, std::string_view pTESSource);
	uint32_t compileShaders(GLuint pType, const std::string& pStr);
	std::string parsePath(const std::filesystem::path& pPath);

private:
	uint32_t mShaderID;
	std::filesystem::path mPathVertex, mPathFragment, mPathGeometry;
	std::unordered_map<std::string, GLuint> mStorageLocations;
};

