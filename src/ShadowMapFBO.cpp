#include "ShadowMapFBO.h"

ShadowMapFBO::ShadowMapFBO()
{
}

ShadowMapFBO::~ShadowMapFBO()
{
	glDeleteFramebuffers(1, &mFBO);
}

void ShadowMapFBO::init(uint32_t pWidth, uint32_t pHeight)
{
	mWidth = pWidth;
	mHeight = pHeight;

	glGenFramebuffers(1, &mFBO);

	glGenTextures(1, &mShadowMap);
	glBindTexture(GL_TEXTURE_2D, mShadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, pWidth, pHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << std::format("Frame buffer is not completed! Status: {}\n", status);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapFBO::bindWrite()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, mWidth, mHeight);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
}

void ShadowMapFBO::bindRead(GLenum pTextureUnit)
{
	glActiveTexture(pTextureUnit);
	glBindTexture(GL_TEXTURE_2D, mShadowMap);
}

void ShadowMapFBO::unbind(uint32_t pWinWidth, uint32_t pWinHeight)
{
	glBindBuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, pWinWidth, pWinHeight);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}