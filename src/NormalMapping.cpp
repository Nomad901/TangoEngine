#include "NormalMapping.h"

NormalMapping::NormalMapping(const std::filesystem::path& pVertShader,
							 const std::filesystem::path& pFragShader,
							 const std::filesystem::path& pDiffuseTexturePath, 
							 const std::filesystem::path& pNormalTexturePath, 
							 TypeOfPrimitive pTypeOfPrimitive)
{
	init(pVertShader, pFragShader, pDiffuseTexturePath, pNormalTexturePath, pTypeOfPrimitive);
}

void NormalMapping::init(const std::filesystem::path& pVertShader,
						 const std::filesystem::path& pFragShader, 
						 const std::filesystem::path& pDiffuseTexturePath,
						 const std::filesystem::path& pNormalTexturePath,
						 TypeOfPrimitive pTypeOfPrimitive)
{
	mDiffuseTexture.init(pDiffuseTexturePath);
	mDiffuseTexture.setTarget(GL_TEXTURE_2D);	
	mNormalTexture.init(pNormalTexturePath);
	mNormalTexture.setTarget(GL_TEXTURE_2D);

	std::shared_ptr<Primitive> primitive;
	switch (pTypeOfPrimitive)
	{
	case NormalMapping::TypeOfPrimitive::QUAD:
		primitive = std::make_shared<Quad>(mDiffuseTexture, 0, true);
		break;
	case NormalMapping::TypeOfPrimitive::TRIANGLE:
		primitive = std::make_shared<Triangle>(mDiffuseTexture, 0, true);
		break;
	case NormalMapping::TypeOfPrimitive::CUBE:
		primitive = std::make_shared<Cube>(mDiffuseTexture, 0, true);
		break;
	case NormalMapping::TypeOfPrimitive::PYRAMID:
		primitive = std::make_shared<Pyramid>(mDiffuseTexture, 0, true);
		break;
	}
	mMesh.init(primitive, true);
	
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

	mDiffuseTexture.bind(0);
	mNormalTexture.bind(1);
	mNormalMappingShader.setUniform1i("uDiffuseMap", 0);
	mNormalMappingShader.setUniform1i("uNormalMap", 1);
}
