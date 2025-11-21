#include "Water.h"

Water::Water(const glm::vec3& pWaterPos)
{
	init(pWaterPos);
}

void Water::init(const glm::vec3& pWaterPos)
{
	mWaterPos = pWaterPos;
}

uint32_t Water::getTileSize() const noexcept
{
	return mTileSize;
}

const glm::vec3& Water::getWaterPos() const noexcept
{
	return mWaterPos;
}
