#include "VertexAOS.h"

VertexAOS::VertexAOS(const glm::vec3& pPos, const glm::vec3& pNormal,
						   glm::vec2 pTexCoord, const glm::vec4& pColor)
{
	init(pPos, pNormal, pTexCoord, pColor);
}

void VertexAOS::init(const glm::vec3& pPos, const glm::vec3& pNormal,
						   glm::vec2 pTexCoord, const glm::vec4& pColor)
{
	mPos = pPos;
	mNormal = pNormal;
	mTexCoord = pTexCoord;
	mColor = pColor;

	mNumberOfVertices++;
}

void VertexAOS::addVertex(const glm::vec3& pPos, const glm::vec3& pNormal, 
								glm::vec2 pTexCoords, const glm::vec4& pColor)
{
	mPos = pPos;
	mNormal = pNormal;
	mTexCoord = pTexCoords;
	mColor = pColor;
}

size_t VertexAOS::getNumberOfVertices() const noexcept
{
	return mNumberOfVertices;
}

void VertexAOS::clearAllData()
{
	mPos = glm::vec3(0.0f);
	mNormal = glm::vec3(0.0f);
	mTexCoord = glm::vec2(0.0f);
	mColor = glm::vec4(0.0f);
}

void VertexAOS::setPos(const glm::vec3& pPos)
{
	mPos = pPos;
}

void VertexAOS::setNormal(const glm::vec3& pNormal)
{
	mNormal = pNormal;
}

void VertexAOS::setTexCoord(glm::vec2 pTexCoord)
{
	mTexCoord = pTexCoord;
}

void VertexAOS::setColor(const glm::vec4& pColor)
{
	mColor = pColor;
}

const glm::vec3& VertexAOS::getPos() const noexcept
{
	return mPos;
}

const glm::vec3& VertexAOS::getNormal() const noexcept
{
	return mNormal;
}

glm::vec2 VertexAOS::getTexCoord() const noexcept
{
	return mTexCoord;
}

const glm::vec4& VertexAOS::getColor() const noexcept
{
	return mColor;
}
