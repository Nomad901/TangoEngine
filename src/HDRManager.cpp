#include "HDRManager.h"

HDRManager::HDRManager(uint32_t pScreenWidth, uint32_t pScreenHeight,
					   const std::vector<glm::vec3>& pLightPos,
					   const std::vector<glm::vec3>& pLightColors)
{
	init(pScreenWidth, pScreenHeight, pLightPos, pLightColors);
}

void HDRManager::init(uint32_t pScreenWidth, uint32_t pScreenHeight,
					  const std::vector<glm::vec3>& pLightPos,
					  const std::vector<glm::vec3>& pLightColors)
{
	mScreenWidth  = pScreenWidth;
	mScreenHeight = pScreenHeight;
	mLightPositions = pLightPos;
	mLightColors = pLightColors;
	
	initFBO();
	createQuad();
}

void HDRManager::initShaders(const std::filesystem::path& pModelVertPath,
							 const std::filesystem::path& pModelFragPath,
							 const std::filesystem::path& pHDRVertPath, 
							 const std::filesystem::path& pHDRFragPath)
{
	mModelShader.init(pModelVertPath, pModelFragPath);
	mHDRShader.init(pHDRVertPath, pHDRFragPath);

	mShadersAreInitialized = true;
}

Shader& HDRManager::getModelShader() noexcept
{
	return mModelShader;
}

Shader& HDRManager::getHDRShader() noexcept
{
	return mHDRShader;
}

bool HDRManager::shadersAreInitialized() const noexcept
{
	return mShadersAreInitialized;
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
