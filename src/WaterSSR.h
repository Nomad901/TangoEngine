#pragma once
#include <iostream>
#include <format>
#include <vector>

#include "WaterShader.h"
#include "WaterGBuffer.h"
#include "Water.h"

class WaterSSR
{
public:
	WaterSSR() = default;
	WaterSSR(uint32_t pScreenWidth, uint32_t pScreenHeight,
			 const std::filesystem::path& pVertPath,
			 const std::filesystem::path& pFragPath);

	void init(uint32_t pScreenWidth, uint32_t pScreenHeight,
			  const std::filesystem::path& pVertPath,
			  const std::filesystem::path& pFragPath);
	
	void startFrame();
	void endFrame();

	void renderWaterSSR(const std::vector<Water>& pWaterTiles, 
						const glm::mat4& pViewMatrix, const glm::mat4& pProjection);
	
private:
	void bindShader(const glm::mat4& pViewMatrix, const glm::mat4& pProjection);

private:
	WaterGBuffer mWaterGBuffer;
	WaterShader mWaterShader;

};