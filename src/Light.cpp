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

void Light::disableAttenuation()
{
	mAttenuation.mAttenuationIsUp = false;
}

bool Light::attenuationIsEnabled() const noexcept
{
	return mAttenuation.mAttenuationIsUp;
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
