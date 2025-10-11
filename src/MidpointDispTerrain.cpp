#include "MidpointDispTerrain.h"

void MidpointDispTerrain::createMidpointDispTerrain(int32_t pSize, float pRoughness, float pMinHeight, float pMaxHeight)
{
	if (pRoughness < 0.0f)
	{
		std::cout << "Roughness can not be less than zero!\n";
		return;
	}
	mTerrainSize = pSize;
	mMinHeight = pMinHeight;
	mMaxHeight = pMaxHeight;
	Terrain::setHeights(pMinHeight + pMaxHeight / 2, pMaxHeight / 2, pMaxHeight / 2 + pMaxHeight / 3, pMaxHeight);

	mHeightMap.resize(mTerrainSize);
	for (size_t i = 0; i < mTerrainSize; ++i)
	{
		mHeightMap[i].resize(mTerrainSize);
		for (size_t j = 0; j < mTerrainSize; ++j)
		{
			mHeightMap[i][j] = 0.0f;
		}
	}

	createMidpointDispF32(pRoughness);
	normalize(pMinHeight, pMaxHeight);
	mTriangleList.createTriangleList(mTerrainSize, mTerrainSize, this);
}

void MidpointDispTerrain::createMidpointDispF32(float pRoughness)
{
	int32_t rectSize = Utils::getInstance().calcNextPowerOfTwo(mTerrainSize);
	float curHeight = (float)rectSize / 2.0f;
	float heightReduce = pow(2.0f, -pRoughness);

	while (rectSize > 0)
	{
		diamondStep(rectSize, curHeight);
		squareStep(rectSize, curHeight);

		rectSize /= 2;
		curHeight *= heightReduce;
	}
}

void MidpointDispTerrain::diamondStep(int32_t pRectSize, float pCurrHeight)
{
	int32_t halfRectSize = pRectSize / 2;
	for (int32_t y = 0; y < mTerrainSize; y += pRectSize)
	{
		for (int32_t x = 0; x < mTerrainSize; x += pRectSize)
		{
			int32_t nextX = (x + pRectSize) % mTerrainSize;
			int32_t nextY = (y + pRectSize) % mTerrainSize;

			float topLeft = mHeightMap[x][y];
			float topRight = mHeightMap[nextX][y];
			float bottomLeft = mHeightMap[x][nextY];
			float bottomRight = mHeightMap[nextX][nextY];

			int32_t midX = (x + halfRectSize) % mTerrainSize;
			int32_t midY = (y + halfRectSize) % mTerrainSize;

			float randValue = Utils::getInstance().randomFloatRange(-pCurrHeight, pCurrHeight);
			float midPoint = (topLeft + topRight + bottomLeft + bottomRight) / 4.0f;
			mHeightMap[midX][midY] = midPoint + randValue;
		}
	}
}

void MidpointDispTerrain::squareStep(int32_t pRectSize, float pCurrHeight)
{
	int32_t halfRectSize = pRectSize / 2;
	if (halfRectSize == 0)
		return;
	for (int32_t y = 0; y < mTerrainSize; y += halfRectSize)
	{
		for (int32_t x = 0; x < mTerrainSize; x += halfRectSize)
		{
			int32_t nextX = (x + pRectSize) % mTerrainSize;
			int32_t nextY = (y + pRectSize) % mTerrainSize;

			int32_t midX = (x + halfRectSize) % mTerrainSize;
			int32_t midY = (y + halfRectSize) % mTerrainSize;

			int32_t prevMidX = (x - halfRectSize + mTerrainSize) % mTerrainSize;
			int32_t prevMidY = (y - halfRectSize + mTerrainSize) % mTerrainSize;

			float currTopLeft = mHeightMap[x][y];
			float currTopRight = mHeightMap[nextX][y];
			float currCenter = mHeightMap[midX][midY];
			float prevYCenter = mHeightMap[midX][prevMidY];
			float currBottomLeft = mHeightMap[x][nextY];
			float prevXCenter = mHeightMap[prevMidX][midY];

			float currLeftMid = (currTopLeft + currCenter + currBottomLeft + prevXCenter) / 4.0f + Utils::getInstance().randomFloatRange(-pCurrHeight, pCurrHeight);
			float currTopMid = (currTopLeft + currCenter + currTopRight + prevYCenter) / 4.0f + Utils::getInstance().randomFloatRange(-pCurrHeight, pCurrHeight);

			mHeightMap[midX][y] = currTopMid;
			mHeightMap[x][midY] = currLeftMid;
		}
	}
}
