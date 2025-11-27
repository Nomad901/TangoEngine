#include "WaterFBO.h"

WaterFBO::WaterFBO(uint32_t pWindowWidth, uint32_t pWindowHeight,
				   glm::vec2 pReflectionPos, glm::vec2 pReflectionSize,
				   glm::vec2 pRefractionPos, glm::vec2 pRefractionSize)
{
	init(pWindowWidth, pWindowHeight, 
		 pReflectionPos, pReflectionSize, 
		 pRefractionPos, pRefractionSize);
}

void WaterFBO::init(uint32_t pWindowWidth, uint32_t pWindowHeight, 
				    glm::vec2 pReflectionPos, glm::vec2 pReflectionSize,
				    glm::vec2 pRefractionPos, glm::vec2 pRefractionSize)
{
	mReflectionFBO.init(pWindowWidth, pWindowHeight, pReflectionPos, pReflectionSize, true);
	mRefractionFBO.init(pWindowWidth, pWindowHeight, pRefractionPos, pRefractionSize, true);
}

void WaterFBO::destroy()
{
	mReflectionFBO.destroy();
	mRefractionFBO.destroy();
}

void WaterFBO::bindReflectionFBO() noexcept
{
	mReflectionFBO.bind();
}

void WaterFBO::bindRefractionFBO() noexcept
{
	mRefractionFBO.bind();
}

void WaterFBO::unbindCurrentFBO() noexcept
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FBO& WaterFBO::getReflectionFBO()
{
	return mReflectionFBO;
}

FBO& WaterFBO::getRefractionFBO()
{
	return mRefractionFBO;
}

Texture2& WaterFBO::getReflectionTexture()
{
	return mReflectionFBO.getTexture();
}

Texture2& WaterFBO::getRefractionTexture()
{
	return mRefractionFBO.getTexture();
}

Texture2& WaterFBO::getReflectionDepthTexture()
{
	return mReflectionFBO.getDepthTexture();
}

Texture2& WaterFBO::getRefractionDepthTexture()
{
	return mRefractionFBO.getDepthTexture();
}
