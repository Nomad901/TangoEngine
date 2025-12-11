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

struct VertexWithTangent
{
	glm::vec3 mPos{ glm::vec3(0.0f) };
	glm::vec3 mNormal{ glm::vec3(0.0f) };
	glm::vec2 mTexCoord{ glm::vec2(0.0f) };
	glm::vec3 mTangent{ glm::vec3(0.0f) };
	glm::vec3 mBitangent{ glm::vec3(0.0f) };

	static void calculateTangAndBitanForTriangle(VertexWithTangent& pVertexToAssign1,
												 VertexWithTangent& pVertexToAssign2,
												 VertexWithTangent& pVertexToAssign3)
	{
		glm::vec3 edge1 = pVertexToAssign2.mPos - pVertexToAssign1.mPos;
		glm::vec3 edge2 = pVertexToAssign3.mPos - pVertexToAssign1.mPos;

		glm::vec2 deltaUV1 = pVertexToAssign2.mTexCoord - pVertexToAssign1.mTexCoord;
		glm::vec2 deltaUV2 = pVertexToAssign3.mTexCoord - pVertexToAssign1.mTexCoord;

		float determinant = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent, bitangent;

		tangent.x = determinant * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = determinant * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = determinant * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		bitangent.x = determinant * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = determinant * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = determinant * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		
		pVertexToAssign1.mTangent = tangent; pVertexToAssign1.mBitangent = bitangent;
		pVertexToAssign2.mTangent = tangent; pVertexToAssign2.mBitangent = bitangent;
		pVertexToAssign3.mTangent = tangent; pVertexToAssign3.mBitangent = bitangent;
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
	void init(const std::vector<VertexWithTangent>& pVerticesWithTangent, GLenum pUsage);
	void init(const void* pData, GLuint pSize, GLenum pUsage);

	void destroy();
	void generate(uint32_t pCount, uint32_t* pBuffers);
	void setID(uint32_t pRendererID);
	void bind() const;
	void unbind() const;

	uint32_t getID() const noexcept;

private:
	uint32_t mRendererID;

};

