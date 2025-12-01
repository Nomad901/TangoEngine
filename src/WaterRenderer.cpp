#include "WaterRenderer.h"

WaterRenderer::WaterRenderer(const std::filesystem::path& pVertPath,
							 const std::filesystem::path& pFragPath,
							 const std::filesystem::path& pDuDvMap,
							 const std::filesystem::path& pNormalMap)
{
	init(pVertPath, pFragPath, pDuDvMap, pNormalMap);
}

void WaterRenderer::init(const std::filesystem::path& pVertPath,
						 const std::filesystem::path& pFragPath, 
						 const std::filesystem::path& pDuDvMap,
						 const std::filesystem::path& pNormalMap)
{
	mWaterShader.init(pVertPath, pFragPath);
	setUpQuad();
	mDuDvWaterTexture.init(pDuDvMap, true);
	mDuDvWaterTexture.setTarget(GL_TEXTURE_2D);
	mNormalMap.init(pNormalMap, true);
	mNormalMap.setTarget(GL_TEXTURE_2D);
}

void WaterRenderer::render(const std::vector<Water> pWaterTiles, 
						   Camera& pCamera, const glm::mat4& pProjMat, 
						   WaterFBO& pWaterFBO, DirectionalLight& pDirectionalLight,
						   float pNearPlane, float pFarPlane,
						   const glm::vec4& pWaterColor)
{
	bind(pCamera, pProjMat, pWaterFBO, pDirectionalLight, pNearPlane, pFarPlane, pWaterColor);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mMoveFactor += 0.1f;

	float waterWavesTime = fmod(mMoveFactor * WAVE_SPEED, 1.0f);
	
	for (auto& i : pWaterTiles)
	{
		mQuadTransform.setLocalPosition(i.getWaterPos());
		mQuadTransform.setLocalRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		mQuadTransform.setLocalScale(i.getTileSize());

		mWaterShader.setMatrixUniform4fv("uModel", mQuadTransform.getModelMatrix());
	
		mWaterShader.setMoveFactor("uMoveFactor", waterWavesTime);
		mWaterShader.setWaterRoughness("uWaterRoughness", i.getWaterRoughness());
		mWaterShader.setWaterMetallic("uWaterMetallic", i.getWaterMetallic());

		renderQuad();
	}
}

void WaterRenderer::bind(Camera& pCamera, const glm::mat4& pProjMat,
						 WaterFBO& pWaterFBO, DirectionalLight& pDirectionalLight,
						 float pNearPlane, float pFarPlane,
						 const glm::vec4& pWaterColor)
{
	mWaterShader.bind();
	mWaterShader.setMatrixUniform4fv("uProj", pProjMat);
	mWaterShader.setMatrixUniform4fv("uView", pCamera.getViewMatrix());
	mWaterShader.setUniform3fv("uCameraPos", pCamera.getPos());
	mWaterShader.setReflectionTexture("uReflectionTexture", pWaterFBO);
	mWaterShader.setRefractionTexture("uRefractionTexture", pWaterFBO);
	mWaterShader.setDuDvMap("uDuDvMap", mDuDvWaterTexture);
	mWaterShader.setNormalMap("uNormalMap", mNormalMap);
	mWaterShader.setDepthMap("uDepthMap", pWaterFBO.getRefractionDepthTexture());
	mWaterShader.setLightColor("uLightColor", pDirectionalLight.getColor());
	mWaterShader.setLightPos("uLightPos", pDirectionalLight.getPosLight());
	mWaterShader.setNearPlane("uNearPlane", pNearPlane);
	mWaterShader.setFarPlane("uFarPlane", pFarPlane);
	mWaterShader.setWaterColor("uWaterColor", pWaterColor);
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
