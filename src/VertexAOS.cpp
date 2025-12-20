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

void VertexAOS::init(const glm::vec3& pPos, const glm::vec3& pNormal,
					 glm::vec2 pTexCoord, const glm::vec3& pTangent, 
					 const glm::vec3& pBitangent)
{
	mPos = pPos;
	mNormal = pNormal;
	mTexCoord = pTexCoord;
	mColor = glm::vec4(0.0f);
	mTangent = pTangent;
	mBitangent = pBitangent;

	mNumberOfVertices++;
}

void VertexAOS::addVertex(glm::vec3&& pPos, glm::vec3&& pNormal,
						  glm::vec2&& pTexCoords, glm::vec4&& pColor)
{
	mPos = std::move(pPos);
	mNormal = std::move(pNormal);
	mTexCoord = std::move(pTexCoords);
	mColor = std::move(pColor);
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
	mTangent = glm::vec3(0.0f);
	mBitangent = glm::vec3(0.0f);
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

void VertexAOS::setTangent(const glm::vec3& pTangent)
{
	mTangent = pTangent;
}

void VertexAOS::setBitangent(const glm::vec3& pBitangent)
{
	mBitangent = pBitangent;
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

const glm::vec3& VertexAOS::getTangent() const noexcept
{
	return mTangent;
}

const glm::vec3& VertexAOS::getBitangent() const noexcept
{
	return mBitangent;
}