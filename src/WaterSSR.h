#pragma once
#include <iostream>
#include <format>
#include <vector>

#include "WaterShader.h"

class WaterSSR
{
public:
	WaterSSR() = default;
	WaterSSR(float pMetallic, float pRoughness, uint32_t& pDepthBuffer);
	~WaterSSR();

	void init(float pMetallic, float pRoughness, uint32_t& pDepthBuffer);

	void update();

private:
	float mWaterMetallicComponent{};
	float mWaterRoughnessComponent{};
	uint32_t* mDepthBuffer{ nullptr };

};

