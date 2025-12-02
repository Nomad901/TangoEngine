#pragma once
#include <iostream>
#include <format>
#include <vector>

#include "WaterShader.h"
#include "WaterGBuffer.h"

class WaterSSR
{
public:
	WaterSSR() = default;
	WaterSSR();

	void startFrame();
	void endFrame();

	void update(const glm::mat4& pViewMatrix, const glm::mat4& pProjection);

private:
	void bindAll(const glm::mat4& pViewMatrix, const glm::mat4& pInvertexViewMatrix, 
			     const glm::mat4& pProjection, const glm::mat4& pInvertexProjection);
private:
	WaterGBuffer mWaterGBuffer;

};