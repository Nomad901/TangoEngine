#pragma once
#include <vector>

#include "BaseVertex.h"

class VertexSOA : BaseVertex
{
public:
	VertexSOA(uint32_t pCapacity = 100);

	void addVertex(const glm::vec3& pPos, const glm::vec3& pNormal,
						 glm::vec2 pTexCoords, const glm::vec4& pColor) override;
	void getVertex(size_t pIndex, glm::vec3& pPos, glm::vec3& pNormal,
								  glm::vec2& pTexCoord, glm::vec4& pColor);
	void updateVertex(size_t pIndex, const glm::vec3& pPos, const glm::vec3& pNormal,
										   glm::vec2 pTexCoord, const glm::vec4& pColor);

	const std::vector<float> getPositionsX() const noexcept;
	const std::vector<float> getPositionsY() const noexcept;
	const std::vector<float> getPositionsZ() const noexcept;
	
	const std::vector<float> getNormalsX() const noexcept;
	const std::vector<float> getNormalsY() const noexcept;
	const std::vector<float> getNormalsZ() const noexcept;

	const std::vector<float> getTexCoordsX() const noexcept;
	const std::vector<float> getTexCoordsY() const noexcept;

	const std::vector<float> getColorsX() const noexcept;
	const std::vector<float> getColorsY() const noexcept;
	const std::vector<float> getColorsZ() const noexcept;
	const std::vector<float> getColorsW() const noexcept;

	size_t getNumberOfVertices() const noexcept override;
	void clearAllData() override;
	bool isEmpty() const noexcept;

private:
	void reserveSpace();

private:
	std::vector<float> mPosX;
	std::vector<float> mPosY;
	std::vector<float> mPosZ;

	std::vector<float> mNormalX;
	std::vector<float> mNormalY;
	std::vector<float> mNormalZ;

	std::vector<float> mTexCoordX;
	std::vector<float> mTexCoordY;

	std::vector<float> mColorX;
	std::vector<float> mColorY;
	std::vector<float> mColorZ;
	std::vector<float> mColorW;

	size_t mNumberOfVertices;
};

