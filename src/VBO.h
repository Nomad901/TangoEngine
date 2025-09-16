#pragma once
#include <iostream>
#include <vector>

#include "glm/glm.hpp"
#include "glad/glad.h"

struct Vertex
{
	glm::vec3 mPos;
	glm::vec3 mNormals;
	glm::vec4 mColor;
	glm::vec2 mTexCoord;
};

class VBO
{
public:
	VBO() = default;
	VBO(const std::vector<Vertex>& pVertices, GLenum pUsage);
	VBO(const void* pData, GLuint pSize, GLenum pUsage);
	~VBO();

	void init(const std::vector<Vertex>& pVertices, GLenum pUsage);
	void init(const void* pData, GLuint pSize, GLenum pUsage);

	void bind() const;
	void unbind() const;

private:
	uint32_t mRendererID;

};

