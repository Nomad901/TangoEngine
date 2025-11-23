#include "WaterRenderer.h"

WaterRenderer::WaterRenderer(const std::filesystem::path& pVertPath,
							 const std::filesystem::path& pFragPath,
							 const std::filesystem::path& pDuDvMap)
{
	init(pVertPath, pFragPath, pDuDvMap);
}

void WaterRenderer::init(const std::filesystem::path& pVertPath,
						 const std::filesystem::path& pFragPath, 
						 const std::filesystem::path& pDuDvMap)
{
	mWaterShader.init(pVertPath, pFragPath);
	setUpQuad();
	mDuDvWaterTexture.init(pDuDvMap);
	mDuDvWaterTexture.setTarget(GL_TEXTURE_2D);
}

void WaterRenderer::render(const std::vector<Water> pWaterTiles, 
						   Camera& pCamera, const glm::mat4& pProjMat, 
						   WaterFBO& pWaterFBO, float pDeltaTime)
{
	bind(pCamera, pProjMat, pWaterFBO);
	for (auto& i : pWaterTiles)
	{
		mQuadTransform.setLocalPosition(i.getWaterPos());
		mQuadTransform.setLocalRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		mQuadTransform.setLocalScale(i.getTileSize());

		mWaterShader.setMatrixUniform4fv("uModel", mQuadTransform.getModelMatrix());

		mMoveFactor += WAVE_SPEED * pDeltaTime;
		if (mMoveFactor >= 100.0f)
			mMoveFactor = 0.0f;

		mWaterShader.setMoveFactor("uMoveFactor", mMoveFactor);

		renderQuad();
	}
}

void WaterRenderer::bind(Camera& pCamera, const glm::mat4& pProjMat,
						 WaterFBO& pWaterFBO)
{
	mWaterShader.bind();
	mWaterShader.setMatrixUniform4fv("uProj", pProjMat);
	mWaterShader.setMatrixUniform4fv("uView", pCamera.getViewMatrix());
	mWaterShader.setReflectionTexture("uReflectionTexture", pWaterFBO);
	mWaterShader.setRefractionTexture("uRefractionTexture", pWaterFBO);
	mWaterShader.setDuDvMap("uDuDvMap", mDuDvWaterTexture);
}

void WaterRenderer::renderQuad()
{
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void WaterRenderer::setUpQuad()
{
	float quadVertices[] = 
	{
		-1.0f,  1.0f,
		 1.0f, -1.0f,	
		-1.0f, -1.0f,

		 1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f,  1.0f
	};
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
}
