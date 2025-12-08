#include "Vertex.h"

Vertex::Vertex(const glm::vec3& pPos, const glm::vec3& pNormal, 
					 glm::vec2 pTexCoord, const glm::vec4& pColor)
{
	init(pPos, pNormal, pTexCoord, pColor);
}

void Vertex::init(const glm::vec3& pPos, const glm::vec3& pNormal, 
						glm::vec2 pTexCoord, const glm::vec4& pColor)
{
	mPos = pPos;
	mNormal = pNormal;
	mTexCoord = pTexCoord;
	mColor = pColor;
}

void Vertex::setPos(const glm::vec3& pPos)
{
	mPos = pPos;
}

void Vertex::setNormal(const glm::vec3& pNormal)
{
	mNormal = pNormal;
}

void Vertex::setTexCoord(glm::vec2 pTexCoord)
{
	mTexCoord = pTexCoord;
}

void Vertex::setColor(const glm::vec4& pColor)
{
	mColor = pColor;
}

const glm::vec3& Vertex::getPos() const noexcept
{
	return mPos;
}

const glm::vec3& Vertex::getNormal() const noexcept
{
	return mNormal;
}

glm::vec2 Vertex::getTexCoord() const noexcept
{
	return mTexCoord;
}

const glm::vec4& Vertex::getColor() const noexcept
{
	return mColor;
}
