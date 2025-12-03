#include "WaterSSR.h"

WaterSSR::WaterSSR(uint32_t pScreenWidth, uint32_t pScreenHeight,
				   const std::filesystem::path& pVertPath,
				   const std::filesystem::path& pFragPath)
{
	init(pScreenWidth, pScreenHeight, pVertPath, pFragPath);
}

void WaterSSR::init(uint32_t pScreenWidth, uint32_t pScreenHeight,
					const std::filesystem::path& pVertPath,
					const std::filesystem::path& pFragPath)
{
	mWaterGBuffer.init(pScreenWidth, pScreenHeight);
	mWaterShader.init(pVertPath, pFragPath);
	generateWaterMesh();
}

void WaterSSR::startFrame()
{
	mWaterGBuffer.bind(WaterGBuffer::BindingType::WRITE);

	std::array<GLenum, 5> attachments =
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,
	};
	glDrawBuffers(attachments.size(), attachments.data());
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WaterSSR::endFrame()
{
	mWaterGBuffer.unbindForWriting();
}

void WaterSSR::renderWaterSSR(const std::vector<Water>& pWaterTiles, 
							  const glm::mat4& pViewMatrix, const glm::mat4& pProjection)
{
	bindShader(pViewMatrix, pProjection);
	mWaterGBuffer.bind(WaterGBuffer::BindingType::USUAL);

	for (auto& i : pWaterTiles)
	{
		mTransform.setLocalPosition(i.getWaterPos());
		mTransform.setLocalRotation(glm::vec3(0.0f));
		mTransform.setLocalScale(i.getTileSize());

		mWaterShader.setMatrixUniform4fv("uModel", mTransform.getModelMatrix());

		renderWaterMesh();
	}
}

void WaterSSR::bindShader(const glm::mat4& pViewMatrix, const glm::mat4& pProjection)
{
	glm::mat4 inverseView = glm::inverse(pViewMatrix);
	glm::mat4 inverseProjection = glm::inverse(pProjection);

	mWaterShader.bind();

	mWaterShader.setMatrixUniform4fv("uView", pViewMatrix);
	mWaterShader.setMatrixUniform4fv("uInvView", inverseView);
	mWaterShader.setMatrixUniform4fv("uProj", pProjection);
	mWaterShader.setMatrixUniform4fv("uInvProjection", inverseProjection);
	
	mWaterShader.setUniform1i("uPosition",	     0);
	mWaterShader.setUniform1i("uFinalImage",     1);
	mWaterShader.setUniform1i("uWaterNormal",    2);
	mWaterShader.setUniform1i("uExtraComponent", 3);
	mWaterShader.setUniform1i("uColorBuffer",	 4);
}

void WaterSSR::renderWaterMesh()
{
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void WaterSSR::generateWaterMesh()
{
	uint32_t VBO = 0;
	float quadVertices[] =
	{
		-1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		 1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f
	};
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}
