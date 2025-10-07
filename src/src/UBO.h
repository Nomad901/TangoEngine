#pragma once
#include <iostream>
#include <format>
#include <unordered_map>
#include <vector>
#include <string>


#include "glm.hpp"
#include "SDL3/SDL.h"
#include "glad/glad.h"

#include "Mesh.h"
#include "Shader.h"

class UBO
{
public:
	UBO() = default;
	//
	// pair - first takes id of the shader / second - takes the name of the uniform 
	//
	UBO(const std::vector<std::pair<uint32_t, std::string_view>>& pUniform,
		uint32_t pIndex, size_t pSize);
	~UBO();
	UBO(const UBO&) = delete;
	UBO& operator=(const UBO&) = delete;
	UBO(UBO&&) = default;
	UBO& operator=(UBO&&) = default;

	//
	// pair - first takes id of the shader / second - takes the name of the uniform 
	//
	void init(const std::vector<std::pair<uint32_t, std::string_view>>& pUniform,
			  uint32_t pIndex, size_t pSize);
	void correlateData(const std::vector<std::pair<uint32_t, std::string_view>>& pUniform,
					   uint32_t pIndex);

	void bind();
	void unbind();
	template<typename T>
	void appendData(uint32_t pOffset, const T& pData);
	template<>
	void appendData<glm::mat4>(uint32_t pOffset, const glm::mat4& pData);

	uint32_t getID() const noexcept;
	uint32_t getUniformID(std::string_view pName);

private:
	uint32_t mUBOid;
	std::unordered_map<std::string, uint32_t> mStorageID;

};

template<typename T>
inline void UBO::appendData(uint32_t pOffset, const T& pData)
{
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, pOffset, sizeof(T), pData);
	unbind();
}

template<>
inline void UBO::appendData(uint32_t pOffset, const glm::mat4& pData)
{
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, pOffset, sizeof(glm::mat4), glm::value_ptr(pData));
	unbind();
}
