#pragma once
#include <iostream>
#include <random>

class HeightsGenerator
{
public:
	HeightsGenerator();

	float generateHeight(float pX, float pZ);

private:
	float getInterpolatedNoise(float pX, float pZ);
	float interpolate(float pFirst, float pSecond, float pBlend);
	float getSmoothNoise(float pX, float pZ);
	float getNoise(float pX, float pZ);

private:
	float mAmplitude{ 70.0f };
	float mRoughness{ 0.1f };
	int32_t mOctaves{ 1 };
	int32_t mSeed{ 0 };

	std::random_device mRandDevice;
	std::mt19937 mGen{ mRandDevice() };
	std::uniform_int_distribution<int32_t> mGenerator{ 0, 1000000000 };

};

