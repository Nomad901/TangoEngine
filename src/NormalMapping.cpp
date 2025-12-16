#include "NormalMapping.h"

NormalMapping::NormalMapping(const std::filesystem::path& pVertShader,
							 const std::filesystem::path& pFragShader,
							 const std::filesystem::path& pDiffuseTexturePath, 
							 const std::filesystem::path& pNormalTexturePath, 
							 TypeOfPrimitive pTypeOfPrimitive)
{
	init(pVertShader, pFragShader, pDiffuseTexturePath, pNormalTexturePath, pTypeOfPrimitive);
}

NormalMapping::NormalMapping(const std::filesystem::path& pVertShader,
							 const std::filesystem::path& pFragShader, 
							 const std::filesystem::path& pDiffuseTexturePath, 
							 const std::filesystem::path& pNormalTexturePath, 
							 const std::filesystem::path& pDepthTexturePath, 
							 TypeOfPrimitive pTypeOfPrimitive,
							 float pHeightScale)
{
	initParallaxMapping(pVertShader, pFragShader, pDiffuseTexturePath, pNormalTexturePath, pDepthTexturePath, pTypeOfPrimitive, pHeightScale);
}

void NormalMapping::init(const std::filesystem::path& pVertShader,
						 const std::filesystem::path& pFragShader, 
						 const std::filesystem::path& pDiffuseTexturePath,
						 const std::filesystem::path& pNormalTexturePath,
						 TypeOfPrimitive pTypeOfPrimitive)
{
	mParallaxMapping = false;
	mDiffuseTexture.init(pDiffuseTexturePath, true);
	mNormalTexture.init(pNormalTexturePath, true);
	mNormalTexture.setTarget(GL_TEXTURE_2D);

	switch (pTypeOfPrimitive)
	{
	case NormalMapping::TypeOfPrimitive::QUAD:
		mPrimitive = std::make_shared<Quad>(mDiffuseTexture, 0, true);
		break;
	case NormalMapping::TypeOfPrimitive::TRIANGLE:
		mPrimitive = std::make_shared<Triangle>(mDiffuseTexture, 0, true);
		break;
	case NormalMapping::TypeOfPrimitive::CUBE:
		mPrimitive = std::make_shared<Cube>(mDiffuseTexture, 0, true);
		break;
	case NormalMapping::TypeOfPrimitive::PYRAMID:
		mPrimitive = std::make_shared<Pyramid>(mDiffuseTexture, 0, true);
		break;
	}
	mMesh.init(mPrimitive, true);
	mPrimitive->getSingleTex().setTarget(GL_TEXTURE_2D);
	
	mNormalMappingShader.init(pVertShader, pFragShader);
}

void NormalMapping::initParallaxMapping(const std::filesystem::path& pVertShader,
									    const std::filesystem::path& pFragShader, 
									    const std::filesystem::path& pDiffuseTexturePath,
									    const std::filesystem::path& pNormalTexturePath,
									    const std::filesystem::path& pDepthTexturePath,
									    TypeOfPrimitive pTypeOfPrimitive,
										float pHeightScale)
{
	mParallaxMapping = true;
	mHeightScale = pHeightScale;
	mDiffuseTexture.init(pDiffuseTexturePath, false);
	mNormalTexture.init(pNormalTexturePath, false);
	mNormalTexture.setTarget(GL_TEXTURE_2D);
	mDepthTexture.init(pDepthTexturePath, false);
	mDepthTexture.setTarget(GL_TEXTURE_2D);
	
	switch (pTypeOfPrimitive)
	{
	case NormalMapping::TypeOfPrimitive::QUAD:
		mPrimitive = std::make_shared<Quad>(mDiffuseTexture, 0, true);
		break;
	case NormalMapping::TypeOfPrimitive::TRIANGLE:
		mPrimitive = std::make_shared<Triangle>(mDiffuseTexture, 0, true);
		break;
	case NormalMapping::TypeOfPrimitive::CUBE:
		mPrimitive = std::make_shared<Cube>(mDiffuseTexture, 0, true);
		break;
	case NormalMapping::TypeOfPrimitive::PYRAMID:
		mPrimitive = std::make_shared<Pyramid>(mDiffuseTexture, 0, true);
		break;
	}
	mMesh.init(mPrimitive, true);
	mPrimitive->getSingleTex().setTarget(GL_TEXTURE_2D);

	mNormalMappingShader.init(pVertShader, pFragShader);
}

Shader& NormalMapping::getShader() noexcept
{
	return mNormalMappingShader;
}

Mesh& NormalMapping::getMesh() noexcept
{
	return mMesh;
}

Texture2& NormalMapping::getDiffuseTexture() noexcept
{
	return mDiffuseTexture;
}

Texture2& NormalMapping::getNormalTexture() noexcept
{
	return mNormalTexture;
}

void NormalMapping::render(Transform& pTransform,
						   const glm::mat4& pViewMatrix,
						   const glm::mat4& pProjMatrix,
						   const glm::vec3& pPosLight,
						   const glm::vec3& pPosCamera)
{
	bindAll(pTransform, pViewMatrix, pProjMatrix, pPosLight, pPosCamera);
	glDisable(GL_CULL_FACE);
	mMesh.draw();
	glEnable(GL_CULL_FACE);
}

void NormalMapping::bindAll(Transform& pTransform,
							const glm::mat4& pViewMatrix,
							const glm::mat4& pProjMatrix,
							const glm::vec3& pPosLight,
							const glm::vec3& pPosCamera)
{
	mNormalMappingShader.bind();

	mNormalMappingShader.setMatrixUniform4fv("uModel", pTransform.getModelMatrix());
	mNormalMappingShader.setMatrixUniform4fv("uViewMat", pViewMatrix);
	mNormalMappingShader.setMatrixUniform4fv("uProj", pProjMatrix);

	mNormalMappingShader.setUniform3fv("uLightPos", pPosLight);
	mNormalMappingShader.setUniform3fv("uViewPos", pPosCamera);

	mPrimitive->getSingleTex().bind(0);
	mNormalTexture.bind(1);
	mNormalMappingShader.setUniform1i("uDiffuseMap", 0);
	mNormalMappingShader.setUniform1i("uNormalMap", 1);
	if (mParallaxMapping)
	{
		mDepthTexture.bind(2);
		mNormalMappingShader.setUniform1i("uDepthMap", 2);
		mNormalMappingShader.setUniform1f("uHeightScale", mHeightScale);
	}
}
