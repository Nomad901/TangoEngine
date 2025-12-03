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
	mWaterGBuffer.bind(WaterGBuffer::BindingType::USUAL);
	bindShader(pViewMatrix, pProjection);

	for (auto& i : pWaterTiles)
	{
			
	}
}

void WaterSSR::bindShader(const glm::mat4& pViewMatrix, const glm::mat4& pProjection)
{
	glm::mat4 inverseView = glm::inverse(pViewMatrix);
	glm::mat4 inverseProjection = glm::inverse(pProjection);

	mWaterShader.bind();

	mWaterShader.setMatrixUniform4fv("uView", pViewMatrix);
	mWaterShader.setMatrixUniform4fv("uInvView", inverseView);
	mWaterShader.setMatrixUniform4fv("uProjection", pProjection);
	mWaterShader.setMatrixUniform4fv("uInvProjection", inverseProjection);
	
	mWaterShader.setUniform1i("uFinalImage",     mWaterGBuffer.getGDiffuseBuffer());
	mWaterShader.setUniform1i("uPosition",	     mWaterGBuffer.getGPosBuffer());
	mWaterShader.setUniform1i("uWaterNormal",    mWaterGBuffer.getGNormalBuffer());
	mWaterShader.setUniform1i("uExtraComponent", mWaterGBuffer.getExtraComponentBuffer());
	mWaterShader.setUniform1i("uColorBuffer",	 mWaterGBuffer.getColorBuffer());
}