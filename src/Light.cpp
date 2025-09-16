#include "Light.h"

Light::Light(float pIntensity, const glm::vec3& pColorLight)
{
	init(pIntensity, pColorLight);
}

Light::Light(Light&& pOtherLight) noexcept
{
	mBlockIsVisible = std::move(pOtherLight.mBlockIsVisible);
	mIntensity = std::move(pOtherLight.mIntensity);
	mColorLight = std::move(pOtherLight.mColorLight);
}

Light& Light::operator=(Light&& pOtherLight) noexcept
{
	if (&pOtherLight == this)
		return *this;
	mBlockIsVisible = std::move(pOtherLight.mBlockIsVisible);
	mIntensity = std::move(pOtherLight.mIntensity);
	mColorLight = std::move(pOtherLight.mColorLight);
	return *this;
}

void Light::init(float pIntensity, const glm::vec3& pColorLight)
{
	mIntensity = pIntensity;
	mColorLight = pColorLight;
}

void Light::enableAttenuation(float pConstant, float pLinear, float pQuadratic)
{
	mAttenuation.mAttenuationIsUp = true;
	mAttenuation.mConstant = pConstant;
	mAttenuation.mLinear = pLinear;
	mAttenuation.mQuadratic = pQuadratic;
}

const Light::Attenuation Light::getAttenuationSettings() const noexcept
{
	return mAttenuation;
}

void Light::disableAttenuation()
{
	mAttenuation.mAttenuationIsUp = false;
}

bool Light::attenuationIsEnabled() const noexcept
{
	return mAttenuation.mAttenuationIsUp;
}

void Light::setAmbient(const glm::vec3& pAmbient)
{
	mAmbient = pAmbient;
}

void Light::setDiffuse(const glm::vec3& pDiffuse)
{	
	mDiffuse = pDiffuse;
}

void Light::setSpecular(const glm::vec3& pSpecular)
{
	mSpecular = pSpecular;
}

glm::vec3 Light::getAmbient() const noexcept
{
	return mAmbient;
}

glm::vec3 Light::getDiffuse() const noexcept
{
	return mDiffuse;
}

glm::vec3 Light::getSpecular() const noexcept
{
	return mSpecular;
}

void Light::setIntensity(float pIntensity)
{
	mIntensity = pIntensity;
}

void Light::setColor(const glm::vec3& pColor)
{
	mColorLight = pColor;
}

float Light::getIntensity() const noexcept
{
	return mIntensity;
}

glm::vec3 Light::getColor() const noexcept
{
	return mColorLight;
}

DirectionalLight::DirectionalLight(const glm::vec3& pPosLight, const glm::vec3& pDirection,
								   float pIntensity, const glm::vec3& pColorLight)
{
	init(pPosLight, pDirection, pIntensity, pColorLight);
}

DirectionalLight::DirectionalLight(const glm::vec3& pPosLight, const glm::vec3& pDirection, 
								   float pConstant, float pLinear, float pQuadratic, 
								   float pIntensity, const glm::vec3& pColorLight)
{
	init(pPosLight, pDirection, pConstant, pLinear, pQuadratic, pIntensity, pColorLight);
}

void DirectionalLight::init(const glm::vec3& pPosLight, const glm::vec3& pDirection, 
							float pIntensity, const glm::vec3& pColorLight)
{
	mPos = pPosLight;
	mDirection = pPosLight;
	Light::init(pIntensity, pColorLight);
}

void DirectionalLight::init(const glm::vec3& pPosLight, const glm::vec3& pDirection, 
							float pConstant, float pLinear, float pQuadratic, 
							float pIntensity, const glm::vec3& pColorLight)
{
	mPos = pPosLight;
	mDirection = pPosLight;
	Light::init(pIntensity, pColorLight);
	Light::enableAttenuation(pConstant, pLinear, pQuadratic);
}

void DirectionalLight::setPosLight(const glm::vec3& pPos)
{
	mPos = pPos;
}

glm::vec3 DirectionalLight::getPosLight() const noexcept
{
	return mPos;
}

void DirectionalLight::setDirection(const glm::vec3& pDirection)
{
	mDirection = pDirection;
}

glm::vec3 DirectionalLight::getDirection() const noexcept
{
	return mDirection;
}

PointLight::PointLight(const glm::vec3& pPosLight, float pIntensity, const glm::vec3& pColorLight)
{
	init(pPosLight, pIntensity, pColorLight);
}

PointLight::PointLight(const glm::vec3& pPosLight, float pConstant, 
					   float pLinear, float pQuadratic, float pIntensity, 
					   const glm::vec3& pColorLight)
{
	init(pPosLight, pConstant, pLinear, pQuadratic, pIntensity, pColorLight);
}

void PointLight::init(const glm::vec3& pPosLight, float pIntensity, const glm::vec3& pColorLight)
{
	mPosLight = pPosLight;
	Light::init(pIntensity, pColorLight);
}

void PointLight::init(const glm::vec3& pPosLight, float pConstant, 
					  float pLinear, float pQuadratic, float pIntensity, 
					  const glm::vec3& pColorLight)
{
	mPosLight = pPosLight;
	Light::init(pIntensity, pColorLight);
	Light::enableAttenuation(pConstant, pLinear, pQuadratic);
}

void PointLight::setPosLight(const glm::vec3& pPosLight)
{
	mPosLight = pPosLight;
}

glm::vec3 PointLight::getPosLight() const noexcept
{
	return mPosLight;
}

void PointLight::setDirectionLight(const glm::vec3& pDirectionLight)
{
}

glm::vec3 PointLight::getDirectionLight() const noexcept
{
	return glm::vec3();
}
