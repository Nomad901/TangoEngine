#include "WaterGBuffer.h"

WaterGBuffer::WaterGBuffer(uint32_t pScreenWidth, uint32_t pScreenHeight)
{
	init(pScreenWidth, pScreenHeight);
}

WaterGBuffer::~WaterGBuffer()
{
	if (mTextures[0] != 0)
		glDeleteTextures(mTextures.size(), mTextures.data());
}

void WaterGBuffer::init(uint32_t pScreenWidth, uint32_t pScreenHeight)
{
	if (GBuffer::getGBuffer() == 0)
		GBuffer::init(pScreenWidth, pScreenHeight);
	
	GBuffer::bindForWriting();

	glGenTextures(mTextures.size(), mTextures.data());

	generateExtraComponent(pScreenWidth, pScreenHeight);
	generateColorBuffer(pScreenWidth, pScreenHeight);

	GBuffer::unbindForWriting();
}

void WaterGBuffer::bind()
{
	GBuffer::bind();

	uint32_t startTextureUnit = static_cast<uint32_t>(GBUFFER_TEXTURE_TYPE::GBUFFER_NUM_TEXTURES);

	for (size_t i = 0; i < mTextures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + startTextureUnit + static_cast<uint32_t>(i));
		glBindTexture(GL_TEXTURE_2D, mTextures[i]);
	}
}

void WaterGBuffer::unbind()
{
	GBuffer::unbind();
}

uint32_t WaterGBuffer::getExtraComponentID() const noexcept
{
	return mTextures[getIndexTexture(TextureType::EXTRA_COMPONENT_TEX)];
}

uint32_t WaterGBuffer::getColorBufferTex() const noexcept
{
	return mTextures[getIndexTexture(TextureType::COLOR_BUFFER_TEX)];
}

uint32_t WaterGBuffer::getIndexTexture(TextureType pTextureType) const noexcept
{
	return static_cast<uint32_t>(pTextureType);
}

void WaterGBuffer::generateExtraComponent(uint32_t pScreenWidth, uint32_t pScreenHeight)
{
	glBindTexture(GL_TEXTURE_2D, mTextures[getIndexTexture(TextureType::EXTRA_COMPONENT_TEX)]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, pScreenWidth, pScreenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + GBuffer::getFreeTextureAttachment(),
						   GL_TEXTURE_2D, mTextures[getIndexTexture(TextureType::EXTRA_COMPONENT_TEX)], 0);
	GBuffer::setFreeTextureAttachment(GBuffer::getFreeTextureAttachment() + 1);
}

void WaterGBuffer::generateColorBuffer(uint32_t pScreenWidth, uint32_t pScreenHeight)
{
	glBindTexture(GL_TEXTURE_2D, mTextures[getIndexTexture(TextureType::COLOR_BUFFER_TEX)]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, pScreenWidth, pScreenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + GBuffer::getFreeTextureAttachment(),
						   GL_TEXTURE_2D, mTextures[getIndexTexture(TextureType::COLOR_BUFFER_TEX)], 0);
	GBuffer::setFreeTextureAttachment(GBuffer::getFreeTextureAttachment() + 1);
}
