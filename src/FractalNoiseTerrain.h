#pragma once
#include <array>

#include "FastNoiseLite.h"
#include "Terrain.h"

class FractalNoiseTerrain : public Terrain
{
public:
	FractalNoiseTerrain() = default;
	FractalNoiseTerrain(int32_t pSize, float pMinHeight, float pMaxHeight,
						float pAmplitude, float pFrequency, int32_t pOctaves,
						float pLacunarity, float pPersistence);

	void init(int32_t pSize, float pMinHeight, float pMaxHeight,
			  float pAmplitude, float pFrequency, int32_t pOctaves,
			  float pLacunarity, float pPersistence);

private:
	void generateFractalNoise();
	float simplexNoise(float pSampleX, float pSampleY);

private:
	float mMinHeight{ 1.0f };
	float mMaxHeight{ 16.0f };

	float mAmplitude{ mMaxHeight / 2 };
	float mFrequency{ 0.05f };
	int32_t mOctaves{ 3 };

	float mLacunarity{ 2.0f };
	float mPersistence{ 0.5f };

	FastNoiseLite mFastNoiseLiteGenerator;
};

