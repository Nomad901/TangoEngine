#include "FractalNoiseTerrain.h"

FractalNoiseTerrain::FractalNoiseTerrain(int32_t pSize, float pMinHeight, float pMaxHeight,
										 float pAmplitude, float pFrequency, int32_t pOctaves,
										 float pLacunarity, float pPersistence)
{
	init(pSize, pMinHeight, pMaxHeight, pAmplitude, pFrequency, pOctaves, pLacunarity, pPersistence);
}

void FractalNoiseTerrain::init(int32_t pSize, float pMinHeight, float pMaxHeight, 
							   float pAmplitude, float pFrequency, int32_t pOctaves,
							   float pLacunarity, float pPersistence)
{
	mTerrainSize = pSize;
	mMinHeight = pMinHeight;
	mMaxHeight = pMaxHeight;
	mAmplitude = pAmplitude;
	mFrequency = pFrequency;
	mOctaves = pOctaves;
	mLacunarity = pLacunarity;
	mPersistence = pPersistence;
	Terrain::setHeights(pMinHeight + pMaxHeight / 2, pMaxHeight / 2, pMaxHeight / 2 + pMaxHeight / 3, pMaxHeight);

	mFastNoiseLiteGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	mFastNoiseLiteGenerator.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
	mFastNoiseLiteGenerator.SetSeed(1337);
	mFastNoiseLiteGenerator.SetFrequency(mFrequency);
	mFastNoiseLiteGenerator.SetFractalOctaves(mOctaves);
	mFastNoiseLiteGenerator.SetFractalLacunarity(mLacunarity);

	generateFractalNoise();
	finalizeTerrain();
}

void FractalNoiseTerrain::generateFractalNoise()
{
	mHeightMap.clear();
	mHeightMap.resize(mTerrainSize);
	for (size_t i = 0; i < mTerrainSize; ++i)
	{
		mHeightMap[i].resize(mTerrainSize);
		for (size_t j = 0; j < mTerrainSize; ++j)
		{
			float elevation = 0.0f; 
			float tmpFrequency = mFrequency;
			float tmpAmplitude = mAmplitude;

			for (size_t octave = 0; octave < mOctaves; ++octave)
			{
				float sampleX = static_cast<float>(j) * tmpFrequency;
				float sampleY = static_cast<float>(i) * tmpFrequency;
				elevation += simplexNoise(sampleX, sampleY) * tmpAmplitude;
				tmpFrequency *= mLacunarity;
				tmpAmplitude *= mPersistence;
			}
			elevation = std::clamp(std::round(elevation), mMinHeight, mMaxHeight);
			mHeightMap[i][j] = elevation;
		}
	}
}

float FractalNoiseTerrain::simplexNoise(float pSampleX, float pSampleY)
{
	float noiseValue = mFastNoiseLiteGenerator.GetNoise(pSampleX, pSampleY);
	return (noiseValue + 1.0f) * 0.5f;
}
