#include "Material.h"
#include "Texture2.h"

Material::Material(glm::vec3 pAmbient, 
				   glm::vec3 pDiffuse, 
				   glm::vec3 pSpecular, 
				   float pShines)
{
	init(pAmbient, pDiffuse, pSpecular, pShines);
}

void Material::init(glm::vec3 pAmbient, glm::vec3 pDiffuse, glm::vec3 pSpecular, float pShines)
{
	mAmbient = pAmbient;
	mDiffuse = pDiffuse;
	mSpecular = pSpecular;
	mShines = pShines;
}

void Material::sendToShader(Shader& pShader, std::pair<uint32_t, uint32_t> pPosInArrayTex,
							const std::pair<Texture2, Texture2>& pTextures,
							std::pair<uint32_t, uint32_t> pSlots,
							uint32_t pDiffuseIndex, uint32_t pSpecularIndex)
{
	pShader.setUniform3fv("material.ambient", mAmbient);
	pShader.setUniform3fv("material.diffuse", mDiffuse);
	pShader.setUniform3fv("material.specular", mSpecular);
	pShader.setUniform1f("material.shines", mShines);

	pShader.setUniform1i(pTextures.first.getUniformName() + std::to_string(pPosInArrayTex.first), pSlots.first);
	pShader.setUniform1i(pTextures.second.getUniformName() + std::to_string(pPosInArrayTex.second), pSlots.second);

	pShader.setUniform1i("material.diffuseIndex", pDiffuseIndex);
	pShader.setUniform1i("material.specularIndex", pSpecularIndex);
	pShader.setUniform1i("material.isJustColored", 0);
}

void Material::sendToShaderColored(Shader& pShader)
{
	pShader.setUniform3fv("material.ambient", mAmbient);
	pShader.setUniform3fv("material.diffuse", mDiffuse);
	pShader.setUniform3fv("material.specular", mSpecular);
	pShader.setUniform1f("material.shines", mShines);
	pShader.setUniform1i("material.isJustColored", 1);
}

void Material::setAmbient(const glm::vec3& pAmbient)
{
	mAmbient = pAmbient;
}

void Material::setDiffuse(const glm::vec3& pDiffuse)
{
	mDiffuse = pDiffuse;
}

void Material::setSpecular(const glm::vec3& pSpecular)
{
	mSpecular = pSpecular;
}

void Material::setShines(float pShines)
{
	mShines = pShines;
}

glm::vec3 Material::getAmbient() const noexcept
{
	return mAmbient;
}

glm::vec3 Material::getDiffuse() const noexcept
{
	return mDiffuse;
}

glm::vec3 Material::getSpecular() const noexcept
{
	return mSpecular;
}

float Material::getShines() const noexcept
{
	return mShines;
}
