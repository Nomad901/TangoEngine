#include "GBuffer.h"
#include "Shader.h"

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
	
	// generating gbuffer textures and depth buffer
	glGenTextures(mTextures.size(), mTextures.data());
	glGenTextures(1, &mDepthBuffer);
	glGenTextures(1, &mFinalTexture);

	// position texture
	glBindTexture(GL_TEXTURE_2D, mTextures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, pScreenWidth, pScreenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextures[0], 0);
	mCounterOfAttachments++;

	// diffuse texture
	glBindTexture(GL_TEXTURE_2D, mTextures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, pScreenWidth, pScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mTextures[1], 0);
	mCounterOfAttachments++;

	// normal texture
	glBindTexture(GL_TEXTURE_2D, mTextures[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, pScreenWidth, pScreenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mTextures[2], 0);
	mCounterOfAttachments++;

	// depth buffer
	glBindTexture(GL_TEXTURE_2D, mDepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, pScreenWidth, pScreenHeight, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthBuffer, 0);
	mCounterOfAttachments++;

	// final buffer
	glBindTexture(GL_TEXTURE_2D, mFinalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, pScreenWidth, pScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, mFinalTexture, 0);
	mCounterOfAttachments++;

	if (getStatusOfFramebuffer() != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << std::format("FrameBuffer is not completed, status: {}\n", getStatusOfFramebuffer());
		return;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GBuffer::onWindowResize(uint32_t pScreenWidth, uint32_t pScreenHeight)
{
    destroy();
    init(pScreenWidth, pScreenHeight);
}

void GBuffer::startFrame()
{
	//
	// here we set our gbuffer in order to write into this
	// also we choose in which buffer to write - into our final texture which is under GL_COLOR_ATTACHMENT4;
	//
	bindForWriting();

	std::array<GLenum, 3> colorAttachments = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(colorAttachments.size(), colorAttachments.data());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLenum finalAttachment = GL_COLOR_ATTACHMENT4;
	glDrawBuffer(finalAttachment);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GBuffer::bindForGeomPass()
{
	//
	// here we set in which buffers we need to write data from geometry pass;
	//
	if (!Utils::getInstance().bufferIsBound(GL_DRAW_FRAMEBUFFER, mGBuffer))
		bindForWriting();

	std::array<GLenum, 3> colorAttachments = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(colorAttachments.size(), colorAttachments.data());
}

void GBuffer::bindForStencilPass()
{
	glDrawBuffer(GL_NONE);
}

void GBuffer::bindForLightPass()
{
	glDrawBuffer(GL_COLOR_ATTACHMENT4);

	for (size_t i = 0; i < mTextures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mTextures[static_cast<uint32_t>(GBUFFER_TEXTURE_TYPE::GBUFFER_POSITION) + i]);
	}
}
	
void GBuffer::bindForFinalPass()
{
	unbindForWriting();
	bindForReading(false);
	glReadBuffer(GL_COLOR_ATTACHMENT4);
}

void GBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mGBuffer);
	for (size_t i = 0; i < mTextures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mTextures[static_cast<uint32_t>(GBUFFER_TEXTURE_TYPE::GBUFFER_POSITION) + i]);
	}
}

void GBuffer::bind(Shader& pShader, const std::vector<std::string_view>& pTextureNames)
{
	glBindFramebuffer(GL_FRAMEBUFFER, mGBuffer);
	if (!Utils::getInstance().shaderIsBound(pShader.getID()))
		pShader.bind();

	for (size_t i = 0; i < mTextures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mTextures[static_cast<uint32_t>(GBUFFER_TEXTURE_TYPE::GBUFFER_POSITION) + i]);
		if(i < pTextureNames.size())
			pShader.setUniform1i(std::string(pTextureNames[i]), i);
	}
}

void GBuffer::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mGBuffer);
}

void GBuffer::bindForReading(bool pWithTextures)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer);
	if (pWithTextures)
	{
		for (size_t i = 0; i < mTextures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mTextures[static_cast<uint32_t>(GBUFFER_TEXTURE_TYPE::GBUFFER_POSITION) + i]);
		}
	}
}

void GBuffer::bindForReading(Shader& pShader, const std::vector<std::string_view>& pTextureNames)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer);
	if (!Utils::getInstance().shaderIsBound(pShader.getID()))
		pShader.bind();

	for (size_t i = 0; i < mTextures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mTextures[static_cast<uint32_t>(GBUFFER_TEXTURE_TYPE::GBUFFER_POSITION) + i]);
		if (i < pTextureNames.size())
			pShader.setUniform1i(std::string(pTextureNames[i]), i);
	}
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

GLenum GBuffer::getStatusOfFramebuffer() const noexcept
{
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	return status;
}

void GBuffer::setFreeTextureAttachment(uint32_t pFreeTextureAttachment)
{
	mCounterOfAttachments = pFreeTextureAttachment;
}

uint32_t GBuffer::getFreeTextureAttachment() const noexcept
{
	return mCounterOfAttachments;
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

uint32_t GBuffer::getGDiffuseBuffer() const noexcept
{
	return mTextures[static_cast<uint32_t>(GBUFFER_TEXTURE_TYPE::GBUFFER_DIFFUSE)];
}

uint32_t GBuffer::getDepthBuffer() const noexcept
{
	return mDepthBuffer;
}
