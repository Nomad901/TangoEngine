#include "Water.h"

Water::Water()
{
	mDudvMap.init(GL_TEXTURE_2D);
	mNormalMap.init(GL_TEXTURE_2D);
}

void Water::init(int32_t pSize, float pWorldSize)
{
	mWaterShader.init();

	mWaterShader.bind();

}

void Water::setWorldHeight(float pHeight)
{
	mHeight = pHeight;
}

void Water::render(const glm::vec3& pCamPos, const glm::mat4& pVP, const glm::vec3& pLightDir)
{
}

void Water::startReflectionPass()
{
}

void Water::stopReflectionPass()
{
}

void Water::startRefractionPass()
{
}

void Water::stopRefractionPass()
{
}

float Water::getHeight() const noexcept
{
	return mHeight;
}

GLint Water::getReflectionFBO() const noexcept
{
	return mReflectionFBO.getRenderBufferID();
}

GLint Water::getRefractionFBO() const noexcept
{
	return mRefractionFBO.getRenderBufferID();
}

GLint Water::getDudvMapTexture() const noexcept
{
	return mDudvMap.getID();;
}
