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

void DirectionalLight::setDirectionLight(const glm::vec3& pDirectionLight)
{
}

glm::vec3 DirectionalLight::getDirectionLight() const noexcept
{
	return glm::vec3();
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

Spotlight::Spotlight(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pRadius, float pIntensity, const glm::vec3& pColorLight)
{
}

Spotlight::Spotlight(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pRadius, float pConstant, float pLinear, float pQuadratic, float pIntensity, const glm::vec3& pColorLight)
{
}

void Spotlight::init(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pRadius, float pIntensity, const glm::vec3& pColorLight)
{
}

void Spotlight::init(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pRadius, float pConstant, float pLinear, float pQuadratic, float pIntensity, const glm::vec3& pColorLight)
{
}

void Spotlight::setRadius(float pRadius)
{
}

float Spotlight::getRadius() const noexcept
{
	return 0.0f;
}

void Spotlight::setCutOff(float pCutOff)
{
}

float Spotlight::getCutOff() const noexcept
{
	return 0.0f;
}

void Spotlight::setDirectionLight(const glm::vec3& pDirectionLight)
{
}

glm::vec3 Spotlight::getDirectionLight() const noexcept
{
	return glm::vec3();
}

void Spotlight::setPosLight(const glm::vec3& pPosLight)
{
}

glm::vec3 Spotlight::getPosLight() const noexcept
{
	return glm::vec3();
}

Flashlight::Flashlight(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pInnerCutOff, float pOuterCutOff, float pIntensity, const glm::vec3& pColorLight)
{
}

void Flashlight::init(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pInnerCutOff, float pOuterCutOff, float pIntensity, const glm::vec3& pColorLight)
{
}

void Flashlight::setOuterCutOff(float pOuterCutOff)
{
}

void Flashlight::setCutOff(float pCutOff)
{
}

float Flashlight::getOuterCutOff() const noexcept
{
	return 0.0f;
}

float Flashlight::getCutOff() const noexcept
{
	return 0.0f;
}

void Flashlight::setPosLight(const glm::vec3& pPosLight)
{
}

glm::vec3 Flashlight::getPosLight() const noexcept
{
	return glm::vec3();
}

void Flashlight::setDirectionLight(const glm::vec3& pDirectionLight)
{
}

glm::vec3 Flashlight::getDirectionLight() const noexcept
{
	return glm::vec3();
}

AreaLight::AreaLight(const glm::vec3& pPosLight, float pIntensity, const glm::vec3& pColorLight)
{
}

AreaLight::AreaLight(const glm::vec3& pPosLight, float pConstant, float pLinear, float pQuadratic, float pIntensity, const glm::vec3& pColorLight)
{
}

void AreaLight::init(const glm::vec3& pPosLight, float pIntensity, const glm::vec3& pColorLight)
{
}

void AreaLight::init(const glm::vec3& pPosLight, float pConstant, float pLinear, float pQuadratic, float pIntensity, const glm::vec3& pColorLight)
{
}

void AreaLight::setPosLight(const glm::vec3& pPosLight)
{
}

glm::vec3 AreaLight::getPosLight() const noexcept
{
	return glm::vec3();
}

void AreaLight::setDirectionLight(const glm::vec3& pDirectionLight)
{
}

glm::vec3 AreaLight::getDirectionLight() const noexcept
{
	return glm::vec3();
}

EmissiveLight::EmissiveLight(const glm::vec3& pPosLight, bool pFlashing, float pIntensity, const glm::vec3& pColorLight)
{
}

void EmissiveLight::init(const glm::vec3& pPosLight, bool pFlashing, float pIntensity, const glm::vec3& pColorLight)
{
}

void EmissiveLight::setFlashing(bool pFlashing)
{
}

bool EmissiveLight::isFlashing() const noexcept
{
	return false;
}

void EmissiveLight::setPosLight(const glm::vec3& pPosLight)
{
}

glm::vec3 EmissiveLight::getPosLight() const noexcept
{
	return glm::vec3();
}

AttenuationLight::AttenuationLight(const glm::vec3& pPosLight, float pConstant, float pLinear, float pQuadratic, float pIntensity, const glm::vec3& pColorLight)
{
}

void AttenuationLight::init(const glm::vec3& pPosLight, float pConstant, float pLinear, float pQuadratic, float pIntensity, const glm::vec3& pColorLight)
{
}

void AttenuationLight::setPosLight(const glm::vec3& pPosLight)
{
}

glm::vec3 AttenuationLight::getPosLight() const noexcept
{
	return glm::vec3();
}

void AttenuationLight::setDirectionLight(const glm::vec3& pDirectionLight)
{
}

glm::vec3 AttenuationLight::getDirectionLight() const noexcept
{
	return glm::vec3();
}
