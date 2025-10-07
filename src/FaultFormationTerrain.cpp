#include "FaultFormationTerrain.h"

void FaultFormationTerrain::createFaultFormation(int32_t pTerrainSize, int32_t pIterations, float pMinHeight, float pMaxHeight, float pFilter)
{
	mTerrainSize = pTerrainSize;
	mMinHeight = pMinHeight;
	mMaxHeight = pMaxHeight;

	mHeightMap.resize(mTerrainSize);
	for (size_t i = 0; i < mTerrainSize; ++i)
	{
		mHeightMap[i].resize(mTerrainSize);
		for (size_t j = 0; j < mTerrainSize; ++j)
		{
			mHeightMap[i][j] = 0.0f;
		}
	}

	createFaultFormationInner(pIterations, pMinHeight, pMaxHeight);
	applyFIRFilter(pFilter);

	normalize(pMinHeight, pMaxHeight);

	mTriangleList.createTriangleList(mTerrainSize, mTerrainSize, this);
}

void FaultFormationTerrain::createFaultFormationInner(int32_t pIterations, float pMinHeight, float pMaxHeight)
{
	float deltaHeight = pMaxHeight - pMinHeight;
	for (int32_t i = 0; i < pIterations; ++i)
	{
		float iterationRatio = ((float)i / (float)pIterations);
		float height = pMaxHeight - iterationRatio * deltaHeight;

		TerrainPoint terrainPoint1, terrainPoint2;
		getRandomTerrainPoints(terrainPoint1, terrainPoint2);

		int32_t dirX = terrainPoint2.mX - terrainPoint1.mX;
		int32_t dirZ = terrainPoint2.mZ - terrainPoint1.mZ;

		for (int32_t x = 0; x < mTerrainSize; ++x)
		{
			for (int32_t z = 0; z < mTerrainSize; ++z)
			{
				int32_t dirX_in = x - terrainPoint1.mX;
				int32_t dirZ_in = z - terrainPoint1.mZ;
				
				float crossProduct = (float)dirX_in * dirZ - (float)dirX * dirZ_in;
				if (crossProduct > 0.0f)
				{
					float curHeight = mHeightMap[x][z];
					mHeightMap[x][z] = curHeight + height;
				}
			}
		}
	}
}

void FaultFormationTerrain::getRandomTerrainPoints(TerrainPoint& pTerrainPoint1, TerrainPoint& pTerrainPoint2)
{
	pTerrainPoint1.mX = rand() % mTerrainSize;
	pTerrainPoint1.mZ = rand() % mTerrainSize;

	uint32_t counter = 0;

	do
	{
		pTerrainPoint2.mX = rand() % mTerrainSize;
		pTerrainPoint2.mZ = rand() % mTerrainSize;
		if (counter++ == 1000)
		{
			std::cout << std::format("An endless loop detected; File: [{}] Line: [{}]\n", __FILE__, __LINE__);
			return;
		}
	} while (pTerrainPoint1.isEqual(pTerrainPoint2));
}

void FaultFormationTerrain::applyFIRFilter(float pFilter)
{
	// left to right
	for (int32_t z = 0; z < mTerrainSize; ++z)
	{
		float prevVal = mHeightMap[0][z];
		for (int32_t x = 0; x < mTerrainSize; ++x)
		{
			prevVal = FIRFilterSinglePoint(x, z, prevVal, pFilter);
		}
	}

	// right to left
	for (int32_t z = 0; z < mTerrainSize; ++z)
	{
		float prevVal = mHeightMap[mTerrainSize - 1][z];
		for (int32_t x = mTerrainSize - 2; x >= 0; --x)
		{
			prevVal = FIRFilterSinglePoint(x, z, prevVal, pFilter);
		}
	}

	//bottom to top
	for (int32_t x = 0; x < mTerrainSize; ++x)
	{
		float prevVal = mHeightMap[x][0];
		for (int32_t z = 1; z < mTerrainSize; ++z)
		{
			prevVal = FIRFilterSinglePoint(x, z, prevVal, pFilter);
		}
	}

	//top to bottom
	for (int32_t x = 0; x < mTerrainSize; ++x)
	{
		float prevVal = mHeightMap[x][mTerrainSize - 1];
		for (int32_t z = mTerrainSize - 2; z >= 0; --z)
		{
			prevVal = FIRFilterSinglePoint(x, z, prevVal, pFilter);
		}
	}
}

float FaultFormationTerrain::FIRFilterSinglePoint(int32_t pX, int32_t pZ, float pPrevFractalVal, float pFilter)
{
	float curVal = mHeightMap[pX][pZ];
	float newVal = pFilter * pPrevFractalVal + (1 - pFilter) * curVal;
	mHeightMap[pX][pZ] = newVal;
	return newVal;
}
