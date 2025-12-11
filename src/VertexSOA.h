#pragma once
#include <vector>
#include <cassert>
#include <optional>

#include "BaseVertex.h"

class VertexSOA : public BaseVertex
{
public:
	VertexSOA(uint32_t pCapacity = 100);

	void addVertex(glm::vec3&& pPos, glm::vec3&& pNormal, glm::vec3&& pTexCoords);
	void addVertex(glm::vec3&& pPos, glm::vec3&& pNormal,
				   glm::vec2&& pTexCoords, glm::vec4&& pColor) override;
	void addVertex(glm::vec3&& pPos, glm::vec3&& pNormal,
				   glm::vec2&& pTexCoords, glm::vec3&& pTangent, 
				   glm::vec3&& pBitangent);

	void getVertex(size_t pIndex, glm::vec3& pPos, glm::vec3& pNormal,
								  glm::vec2& pTexCoord, glm::vec4& pColor) const;
	void updateVertex(size_t pIndex, glm::vec3&& pPos, glm::vec3&& pNormal,
									 glm::vec2&& pTexCoord, glm::vec4&& pColor);

	const std::vector<float>& getPositionsX() const noexcept;
	const std::vector<float>& getPositionsY() const noexcept;
	const std::vector<float>& getPositionsZ() const noexcept;
	
	const std::vector<float>& getNormalsX() const noexcept;
	const std::vector<float>& getNormalsY() const noexcept;
	const std::vector<float>& getNormalsZ() const noexcept;

	const std::vector<float>& getTexCoordsX() const noexcept;
	const std::vector<float>& getTexCoordsY() const noexcept;

	std::optional<const std::vector<float>&> getColorsX() const noexcept;
	std::optional<const std::vector<float>&> getColorsY() const noexcept;
	std::optional<const std::vector<float>&> getColorsZ() const noexcept;
	std::optional<const std::vector<float>&> getColorsW() const noexcept;

	std::optional<const std::vector<float>&> getTangentX() const noexcept;
	std::optional<const std::vector<float>&> getTangentY() const noexcept;
	std::optional<const std::vector<float>&> getTangentZ() const noexcept;

	std::optional<const std::vector<float>&> getBitangentX() const noexcept;
	std::optional<const std::vector<float>&> getBitangentY() const noexcept;
	std::optional<const std::vector<float>&> getBitangentZ() const noexcept;

	size_t getNumberOfVertices() const noexcept override;
	void clearAllData() override;
	void clearExactData(size_t pIndex);

	bool isEmpty(size_t pIndex) const noexcept;
	bool hasColors() const noexcept;
	bool hasTangent() const noexcept;

private:
	void reserveSpace(uint32_t pCapacity);

private:
	std::vector<float> mPosX, mPosY, mPosZ;
	std::vector<float> mNormalX, mNormalY, mNormalZ;
	std::vector<float> mTexCoordX, mTexCoordY;
	std::vector<float> mColorX, mColorY, mColorZ, mColorW;
	std::vector<float> mTangentX, mTangentY, mTangentZ;
	std::vector<float> mBitangentX, mBitangentY, mBitangentZ;

	size_t mNumberOfVertices;
};

