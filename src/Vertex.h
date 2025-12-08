#pragma once
#include <iostream>
#include <format>

#include "glm.hpp"

class Vertex
{
public:
	Vertex() = default;
	Vertex(const glm::vec3& pPos, const glm::vec3& pNormal, 
				 glm::vec2 pTexCoord, const glm::vec4& pColor);

	void init(const glm::vec3& pPos, const glm::vec3& pNormal,
					glm::vec2 pTexCoord, const glm::vec4& pColor);

	void setPos(const glm::vec3& pPos);
	void setNormal(const glm::vec3& pNormal);
	void setTexCoord(glm::vec2 pTexCoord);
	void setColor(const glm::vec4& pColor);

	const glm::vec3& getPos() const noexcept;
	const glm::vec3& getNormal() const noexcept;
	glm::vec2 getTexCoord() const noexcept;
	const glm::vec4& getColor() const noexcept;

private:
	glm::vec3 mPos;
	glm::vec3 mNormal;
	glm::vec2 mTexCoord;
	glm::vec4 mColor;
};

