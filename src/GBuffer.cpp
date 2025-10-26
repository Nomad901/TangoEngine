#include "GBuffer.h"

GBuffer::~GBuffer()
{
	glDeleteFramebuffers(1, &mGBuffer);
}

void GBuffer::init(uint32_t pScreenWidth, uint32_t pScreenHeight)
{
	glGenFramebuffers(1, &mGBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mGBuffer);
	
	glGenTextures(1, &mGPos);
	glBindTexture(GL_TEXTURE_2D, mGPos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, pScreenWidth, pScreenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGPos, 0);

	glGenTextures(1, &mGNormal);
	glBindTexture(GL_TEXTURE_2D, mGNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, pScreenWidth, pScreenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mGNormal, 0);

	glGenTextures(1, &mGColorSpec);
	glBindTexture(GL_TEXTURE_2D, mGColorSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pScreenWidth, pScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mGColorSpec, 0);

	std::array<uint32_t, 3> attachments = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments.data());

	glGenRenderbuffers(1, &mRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, mRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, pScreenWidth, pScreenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << std::format("Frame buffer isnt completed. Id of frameBuffer: {}\n", mGBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mGBuffer);
}

void GBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
	return mGPos;
}

uint32_t GBuffer::getGNormalBuffer() const noexcept
{
	return mGNormal;
}

uint32_t GBuffer::getGColorSpecBuffer() const noexcept
{
	return mGColorSpec;
}

uint32_t GBuffer::getRBOBuffer() const noexcept
{
	return mRBO;
}
