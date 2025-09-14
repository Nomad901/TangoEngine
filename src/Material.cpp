#include "Material.h"
#include "Texture2.h"

Material::Material(glm::vec3 pAmbient, 
				   glm::vec3 pDiffuse, 
				   glm::vec3 pSpecular, 
				   float pShines,
				   std::vector<Texture2>& pTextures)
{
	init(pAmbient, pDiffuse, pSpecular, pShines, pTextures);
}

void Material::init(glm::vec3 pAmbient, glm::vec3 pDiffuse, glm::vec3 pSpecular, float pShines, std::vector<Texture2>& pTextures)
{
	mAmbient = pAmbient;
	mDiffuse = pDiffuse;
	mSpecular = pSpecular;
	mTextures.reserve(pTextures.size());
	mTextures = pTextures;
	mShines = pShines;
}

void Material::sendToShader(Shader& pShader, bool isJustColored)
{
	pShader.setUniform3fv("material.ambient", mAmbient);
	pShader.setUniform3fv("material.diffuse", mDiffuse);
	pShader.setUniform3fv("material.specular", mSpecular);
	pShader.setUniform1f("material.shines", mShines);
	for (int32_t i = 0; i < std::ssize(mTextures) && i < 8; ++i)
	{
		std::string unifString = mTextures[i].getUniformName() + '[' + std::to_string(i) + ']';
		pShader.setUniform1i(unifString, i);
	}
	pShader.setUniform1i("material.diffuseIndex", 0);
	pShader.setUniform1i("material.specularIndex", 1);
	if (isJustColored)
		pShader.setUniform1i("material.isJustColored", 1);
	else 
		pShader.setUniform1i("material.isJustColored", 0);
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

void Material::setCurrentIndex(int32_t pIndex)
{
	mCurrentIndex = pIndex;
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

int32_t Material::getCurrentIndex() const noexcept
{
	return mCurrentIndex;
}
