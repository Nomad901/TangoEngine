#pragma once
#include "BaseVertex.h"

class VertexAOS : public BaseVertex
{
public:
	VertexAOS() = default;
	VertexAOS(const glm::vec3& pPos, const glm::vec3& pNormal,
			  glm::vec2 pTexCoord, const glm::vec4& pColor);

	void init(const glm::vec3& pPos, const glm::vec3& pNormal,
			  glm::vec2 pTexCoord, const glm::vec4& pColor);
	void init(const glm::vec3& pPos, const glm::vec3& pNormal,
			  glm::vec2 pTexCoord, const glm::vec3& pTangent,
			  const glm::vec3& pBitangent);

	// in this class "addVertex" just changes current data of the vertex; 
	void addVertex(glm::vec3&& pPos, glm::vec3&& pNormal,
				   glm::vec2&& pTexCoords, glm::vec4&& pColor) override;

	size_t getNumberOfVertices() const noexcept override;
	void clearAllData() override;

	void setPos(const glm::vec3& pPos);
	void setNormal(const glm::vec3& pNormal);
	void setTexCoord(glm::vec2 pTexCoord);
	void setColor(const glm::vec4& pColor);
	void setTangent(const glm::vec3& pTangent);
	void setBitangent(const glm::vec3& pBitangent);

	const glm::vec3& getPos() const noexcept;
	const glm::vec3& getNormal() const noexcept;
	glm::vec2 getTexCoord() const noexcept;
	const glm::vec4& getColor() const noexcept;
	const glm::vec3& getTangent() const noexcept;
	const glm::vec3& getBitangent() const noexcept;

private:
	glm::vec3 mPos;
	glm::vec3 mNormal;
	glm::vec2 mTexCoord;
	glm::vec4 mColor;
	glm::vec3 mTangent; 
	glm::vec3 mBitangent;

	static inline size_t mNumberOfVertices = 0;
};

