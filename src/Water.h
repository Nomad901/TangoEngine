#pragma once
#include "Texture2.h"
#include "FBO.h"
#include "TriangleList.h"

class Water
{
public:
	Water() = default;
	Water(const glm::vec3& pWaterPos);

	void init(const glm::vec3& pWaterPos);

	uint32_t getTileSize() const noexcept;
	const glm::vec3& getWaterPos() const noexcept;
	
private:
	const uint32_t mTileSize{ 529 };
	glm::vec3 mWaterPos;
};

