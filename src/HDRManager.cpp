#include "HDRManager.h"

HDRManager::HDRManager(uint32_t pScreenWidth, uint32_t pScreenHeight)
{
	init(pScreenWidth, pScreenHeight);
}

void HDRManager::init(uint32_t pScreenWidth, uint32_t pScreenHeight)
{
	mScreenWidth  = pScreenWidth;
	mScreenHeight = pScreenHeight;

	initFBO();
	createQuad();
}

void HDRManager::initShaders(const std::filesystem::path& pHDRVertPath, 
							 const std::filesystem::path& pHDRFragPath)
{
	mHDRShader.init(pHDRVertPath, pHDRFragPath);

	mCurrentState = States::INITIALIZED;
}

void HDRManager::startHDRPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBOid);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mCurrentState = States::STARTED;
}

void HDRManager::stopHDRPass()
{
	if (mCurrentState == States::STARTED)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		mCurrentState = States::STOPED;
	}
}

void HDRManager::renderHDR(bool pTurnOnHDR, float pExposure)
{
	if (mCurrentState == States::STOPED)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		bindAll(pTurnOnHDR, pExposure);
	
		renderQuad();
	}
}

Shader& HDRManager::getHDRShader() noexcept
{
	return mHDRShader;
}

HDRManager::States HDRManager::getCurrentState() const noexcept
{
	return mCurrentState;
}

void HDRManager::bindAll(bool pTurnOnHDR, float pExposure)
{
	mHDRShader.bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mColorBuffer);
	mHDRShader.setUniform1i("uHDRBuffer", 0);
	if (pTurnOnHDR)
		mHDRShader.setUniform1i("uHDR", 1);
	else
		mHDRShader.setUniform1i("uHDR", 0);
	mHDRShader.setUniform1f("uExposure", pExposure);
}

void HDRManager::initFBO()
{
	uint32_t rboDepth = 0;

	glGenFramebuffers(1, &mFBOid);

	glGenTextures(1, &mColorBuffer);
	glBindTexture(GL_TEXTURE_2D, mColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mScreenWidth, mScreenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mScreenWidth, mScreenHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, mFBOid);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer isnt completed!\n";
}

void HDRManager::createQuad()
{
	uint32_t quadVBO = 0;

	float quadVertices[] = 
	{
		-1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
	};
	glGenVertexArrays(1, &mQuadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(mQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void HDRManager::renderQuad()
{
	glBindVertexArray(mQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
