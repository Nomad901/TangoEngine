#include "GBuffer.h"

GBuffer::~GBuffer()
{
	glDeleteFramebuffers(1, &mGBuffer);
	if (mTextures[0] != 0)
		glDeleteTextures(mTextures.size(), mTextures.data());
	if (mDepthBuffer != 0)
		glDeleteTextures(1, &mDepthBuffer);
}

void GBuffer::init(uint32_t pScreenWidth, uint32_t pScreenHeight)
{
	glGenFramebuffers(1, &mGBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mGBuffer);

	glGenTextures(mTextures.size(), mTextures.data());

	for (size_t i = 0; i < mTextures.size(); ++i)
	{
		glBindTexture(GL_TEXTURE_2D, mTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, pScreenWidth, pScreenHeight, 0, GL_RGB, GL_FLOAT, nullptr);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mTextures[i], 0);
	}

	glGenTextures(1, &mDepthBuffer);
	glBindTexture(GL_TEXTURE_2D, mDepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, pScreenWidth, pScreenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthBuffer, 0);

	std::array<GLenum, 4> drawBuffers = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(drawBuffers.size(), drawBuffers.data());

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << std::format("Frame buffer was not completed! Returning... {}\n", status);
		return;
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mGBuffer);
}

void GBuffer::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mGBuffer);
}

void GBuffer::bindForReading()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer);
}

void GBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::unbindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GBuffer::unbindForReading()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void GBuffer::setReadBuffer(GBUFFER_TEXTURE_TYPE pTextureType)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + static_cast<uint32_t>(pTextureType));
}

void GBuffer::destroy()
{
	glDeleteFramebuffers(1, &mGBuffer);
}

uint32_t GBuffer::getGBuffer() const noexcept
{
	return mGBuffer;
}

uint32_t GBuffer::getGPosBuffer() const noexcept
{
	return mTextures[static_cast<uint32_t>(GBUFFER_TEXTURE_TYPE::GBUFFER_POSITION)];
}

uint32_t GBuffer::getGNormalBuffer() const noexcept
{
	return mTextures[static_cast<uint32_t>(GBUFFER_TEXTURE_TYPE::GBUFFER_NORMAL)];
}

uint32_t GBuffer::getGColorSpecBuffer() const noexcept
{
	return mTextures[static_cast<uint32_t>(GBUFFER_TEXTURE_TYPE::GBUFFER_DIFFUSE)];
}

uint32_t GBuffer::getRBOBuffer() const noexcept
{
	return mDepthBuffer;
}
