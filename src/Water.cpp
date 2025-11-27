#include "Water.h"

Water::Water(const glm::vec3& pWaterPos, const glm::vec3& pTileSize)
{
	init(pWaterPos, pTileSize);
}

void Water::init(const glm::vec3& pWaterPos, const glm::vec3& pTileSize)
{
	mWaterPos = pWaterPos;
	mTileSize = pTileSize;
}

void Water::setWaterPos(const glm::vec3& pWaterPos)
{
	mWaterPos = pWaterPos;
}

void Water::setTileSize(const glm::vec3& pTileSize)
{
	mTileSize = pTileSize;
}

void Water::setWaterHeight(float pWaterHeight)
{
	mWaterPos.y = pWaterHeight;
}

const glm::vec3& Water::getTileSize() const noexcept
{
	return mTileSize;
}

const glm::vec3& Water::getWaterPos() const noexcept
{
	return mWaterPos;
}

float Water::getWaterHeight() const noexcept
{
	return mWaterPos.y;
}
