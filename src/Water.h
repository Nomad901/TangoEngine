#pragma once
#include "Texture2.h"
#include "FBO.h"
#include "TriangleList.h"

class Water
{
public:
	Water() = default;
	Water(const glm::vec3& pWaterPos, const glm::vec3& pTileSize);

	void init(const glm::vec3& pWaterPos, const glm::vec3& pTileSize);

	void setWaterPos(const glm::vec3& pWaterPos);
	void setTileSize(const glm::vec3& pTileSize);

	const glm::vec3& getTileSize() const noexcept;
	const glm::vec3& getWaterPos() const noexcept;
	
private:
	glm::vec3 mTileSize;
	glm::vec3 mWaterPos;
};

