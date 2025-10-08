#pragma once
#include "Terrain.h"

class MidpointDispTerrain : public Terrain
{
public:
	MidpointDispTerrain() = default;

	void createMidpointDispTerrain(int32_t pSize, float pRoughness, float pMinHeight, float pMaxHeight);

private:
	void createMidpointDispF32(float pRoughness);
	void diamondStep(int32_t pRectSize, float pCurrHeight);
	void squareStep(int32_t pRectSize, float pCurrHeight);

};

