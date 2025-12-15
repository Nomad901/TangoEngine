#include "NormalMapping.h"

template<typename T>
NormalMapping<T>::NormalMapping(const std::filesystem::path& pVertShader,
								const std::filesystem::path& pFragShader,
								const std::filesystem::path& pDiffuseTexturePath, 
								const std::filesystem::path& pNormalTexturePath)
{
	init(pVertShader, pFragShader, pTypeOfMesh, pDiffuseTexturePath, pNormalTexturePath);
}

template<typename T>
void NormalMapping<T>::init(const std::filesystem::path& pVertShader,
						 const std::filesystem::path& pFragShader, 
						 const std::filesystem::path& pDiffuseTexturePath,
						 const std::filesystem::path& pNormalTexturePath)
{
	mDiffuseTexture.init(pDiffuseTexturePath);
	mDiffuseTexture.setTarget(GL_TEXTURE_2D);	
	mNormalTexture.init(pNormalTexturePath);
	mNormalTexture.setTarget(GL_TEXTURE_2D);

	std::shared_ptr<Primitive> primitive = std::make_shared<T>(mDiffuseTexture, 0, true);
	mMesh.init(primitive);
	
	mNormalMappingShader.init(pVertShader, pFragShader);
}

template<typename T>
Shader& NormalMapping<T>::getShader() noexcept
{
	return mNormalMappingShader;
}

template<typename T>
Mesh& NormalMapping<T>::getMesh() noexcept
{
	return mMesh;
}

template<typename T>
Texture2& NormalMapping<T>::getDiffuseTexture() noexcept
{
	return mDiffuseTexture;
}

template<typename T>
Texture2& NormalMapping<T>::getNormalTexture() noexcept
{
	return mNormalTexture;
}

template<typename T>
void NormalMapping<T>::render(Transform& pTransform,
							  const glm::mat4& pViewMatrix,
							  const glm::mat4& pProjMatrix,
							  const glm::vec3& pPosLight,
							  const glm::vec3& pPosCamera)
{
	bindAll(pTransform, pViewMatrix, pProjMatrix, pPosLight, pPosCamera);
	mMesh.draw();
}

template<typename T>
void NormalMapping<T>::bindAll(Transform& pTransform,
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
