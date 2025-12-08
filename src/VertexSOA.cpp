#include "VertexSOA.h"

VertexSOA::VertexSOA(uint32_t pCapacity)
{
}

void VertexSOA::addVertex(const glm::vec3& pPos, const glm::vec3& pNormal, glm::vec2 pTexCoords, const glm::vec4& pColor)
{
}

void VertexSOA::getVertex(size_t pIndex, glm::vec3& pPos, glm::vec3& pNormal, glm::vec2& pTexCoord, glm::vec4& pColor)
{
}

void VertexSOA::updateVertex(size_t pIndex, const glm::vec3& pPos, const glm::vec3& pNormal, glm::vec2 pTexCoord, const glm::vec4& pColor)
{
}

const std::vector<float> VertexSOA::getPositionsX() const noexcept
{
	return std::vector<float>();
}

const std::vector<float> VertexSOA::getPositionsY() const noexcept
{
	return std::vector<float>();
}

const std::vector<float> VertexSOA::getPositionsZ() const noexcept
{
	return std::vector<float>();
}

const std::vector<float> VertexSOA::getNormalsX() const noexcept
{
	return std::vector<float>();
}

const std::vector<float> VertexSOA::getNormalsY() const noexcept
{
	return std::vector<float>();
}

const std::vector<float> VertexSOA::getNormalsZ() const noexcept
{
	return std::vector<float>();
}

const std::vector<float> VertexSOA::getTexCoordsX() const noexcept
{
	return std::vector<float>();
}

const std::vector<float> VertexSOA::getTexCoordsY() const noexcept
{
	return std::vector<float>();
}

const std::vector<float> VertexSOA::getColorsX() const noexcept
{
	return std::vector<float>();
}

const std::vector<float> VertexSOA::getColorsY() const noexcept
{
	return std::vector<float>();
}

const std::vector<float> VertexSOA::getColorsZ() const noexcept
{
	return std::vector<float>();
}

const std::vector<float> VertexSOA::getColorsW() const noexcept
{
	return std::vector<float>();
}

size_t VertexSOA::getNumberOfVertices() const noexcept
{
	return size_t();
}

void VertexSOA::clearAllData()
{
}

bool VertexSOA::isEmpty() const noexcept
{
	return false;
}

void VertexSOA::reserveSpace()
{
}
