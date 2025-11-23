#pragma once
#include <iostream>

#include "FBO.h"

class WaterFBO
{
public:
	WaterFBO() = default;
	WaterFBO(uint32_t pWindowWidth, uint32_t pWindowHeight, 
			 glm::vec2 pReflectionPos, glm::vec2 pReflectionSize, 
			 glm::vec2 pRefractionPos, glm::vec2 pRefractionSize);

	void init(uint32_t pWindowWidth, uint32_t pWindowHeight,
			  glm::vec2 pReflectionPos, glm::vec2 pReflectionSize,
			  glm::vec2 pRefractionPos, glm::vec2 pRefractionSize);

	void destroy();
	
	void bindReflectionFBO() noexcept;
	void bindRefractionFBO() noexcept;
	void unbindCurrentFBO() noexcept;
	
	FBO& getReflectionFBO();
	FBO& getRefractionFBO();
	Texture2& getReflectionTexture();
	Texture2& getRefractionTexture(); 
	uint32_t getReflectionDepthTexture();
	uint32_t getRefractionDepthTexture();

private:
	FBO mReflectionFBO;
	glm::vec2 mReflectionSize; // glm::vec2(320.0f, 180.0f)
	
	FBO mRefractionFBO;
	glm::vec2 mRefractionSize; 
};

