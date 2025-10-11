#pragma once
#include <iostream>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"

class Terrain;

struct Vertex
{
	glm::vec3 mPos;
	glm::vec3 mNormals{ 0.0f,0.0f,0.0f };
	glm::vec4 mColor;
	glm::vec2 mTexCoord;
	float mLightFactor;

	void initVertex(const Terrain* pTerrain, int32_t pX, int32_t pZ);

	void set(const glm::vec3& pPos, float pPitch, float pHeading)
	{
		mPos = pPos;
		mTexCoord.s = pHeading / 360.0f;
		mTexCoord.t = 0.5f + pPitch / -180.0f;
	}

	void initBySphericalCoords(float pRadius, float pPitch, float pHeading)
	{
		mPos.x = pRadius * cosf(glm::radians(pPitch)) * sinf(glm::radians(pHeading));
		mPos.y = -pRadius * sinf(glm::radians(pPitch));
		mPos.z = pRadius * cosf(glm::radians(pPitch)) * cosf(glm::radians(pHeading));
	}
};

class VBO
{
public:
	VBO() = default;
	VBO(const std::vector<Vertex>& pVertices, GLenum pUsage);
	VBO(const void* pData, GLuint pSize, GLenum pUsage);
	~VBO();
	VBO(const VBO&) = delete;
	VBO& operator=(const VBO&) = delete;
	VBO(VBO&&) = default;
	VBO& operator=(VBO&&) = default;

	void init(const std::vector<Vertex>& pVertices, GLenum pUsage);
	void init(const void* pData, GLuint pSize, GLenum pUsage);

	void destroy();
	void bind() const;
	void unbind() const;

	uint32_t getID() const noexcept;

private:
	uint32_t mRendererID;

};

