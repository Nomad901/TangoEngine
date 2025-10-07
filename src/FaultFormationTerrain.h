#pragma once
#include "Terrain.h"

class FaultFormationTerrain : public Terrain
{
public:
	FaultFormationTerrain() = default;

	void createFaultFormation(int32_t pTerrainSize, int32_t pIterations, float pMinHeight, float pMaxHeight, float pFilter);

private:
	struct TerrainPoint
	{
		void print()
		{
			std::cout << std::format("X: {} / Z: {}\n", mX, mZ);
		}
		bool isEqual(TerrainPoint& pTerrainPointOther)
		{
			return ((mX == pTerrainPointOther.mX) && (mZ == pTerrainPointOther.mZ));
		}

		int32_t mX{ 0 }, mZ{ 0 };
	};

private:
	void createFaultFormationInner(int32_t pIterations, float pMinHeight, float pMaxHeight);
	void getRandomTerrainPoints(TerrainPoint& pTerrainPoint1, TerrainPoint& pTerrainPoint2);
	void applyFIRFilter(float pFilter);
	float FIRFilterSinglePoint(int32_t pX, int32_t pZ, float pPrevFractalVal, float pFilter);
};

