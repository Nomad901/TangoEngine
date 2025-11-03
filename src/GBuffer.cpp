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

	// gbuffer textures
	for (size_t i = 0; i < mTextures.size(); ++i)
	{
		glBindTexture(GL_TEXTURE_2D, mTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, pScreenWidth, pScreenHeight, 0, GL_RGB, GL_FLOAT, nullptr);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mTextures[i], 0);
	}

	// depth buffer
	glBindTexture(GL_TEXTURE_2D, mDepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, pScreenWidth, pScreenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthBuffer, 0);

	// final buffer
	glBindTexture(GL_TEXTURE_2D, mFinalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pScreenWidth, pScreenHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, mFinalTexture, 0);
	
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << std::format("FrameBuffer is not completed, status: {}\n", status);
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
	bindForWriting();
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GBuffer::bindForGeomBuffer()
{
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
		glBindTexture(GL_TEXTURE_2D, mTextures[static_cast<uint32_t>(GBUFFER_TEXTURE_TYPE::GBUFFER_POSITION) + 1]);
	}
}

void GBuffer::bindForFinalPass()
{
	unbindForWriting();
	bindForReading();
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

void GBuffer::bindForReading()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer);
	for (size_t i = 0; i < mTextures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mTextures[static_cast<uint32_t>(GBUFFER_TEXTURE_TYPE::GBUFFER_POSITION) + i]);
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

uint32_t GBuffer::getRBOBuffer() const noexcept
{
	return mDepthBuffer;
}
