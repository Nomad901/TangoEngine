#pragma once
#include <iostream>
#include <format>

#include "glad/glad.h"
#include "SDL3/SDL.h"
#include "glm.hpp"

class ShadowMapFBO
{
public:
	ShadowMapFBO();
	~ShadowMapFBO();

	void init(uint32_t pWidth, uint32_t pHeight);
	
	void bindWrite();
	void bindRead(GLenum pTextureUnit);
	void unbind(uint32_t pWinWidth, uint32_t pWinHeight);

private:
	uint32_t mWidth{}, mHeight{};
	uint32_t mFBO;
	uint32_t mShadowMap;
};

