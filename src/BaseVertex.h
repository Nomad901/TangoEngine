#pragma once
#include <iostream>

#include "glm.hpp"

class BaseVertex
{
public:
	virtual void addVertex(const glm::vec3& pPos, const glm::vec3& pNormal,
								 glm::vec2 pTexCoords, const glm::vec4& pColor) = 0;

	virtual size_t getNumberOfVertices() const noexcept = 0;
	virtual void clearAllData() = 0;
};

