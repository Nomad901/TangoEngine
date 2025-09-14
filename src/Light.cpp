#include "Light.h"

Light::Light(float pIntensity, const glm::vec3& pColorLight, bool pCreateBlockSourceLight)
{
	init(pIntensity, pColorLight, pCreateBlockSourceLight);
}

Light::Light(Light&& pOtherLight) noexcept
{
	mBlockIsVisible = std::move(pOtherLight.mBlockIsVisible);
	mIntensity = std::move(pOtherLight.mIntensity);
	mColorLight = std::move(pOtherLight.mColorLight);
	mBlockLight = std::move(pOtherLight.mBlockLight);
}

Light& Light::operator=(Light&& pOtherLight) noexcept
{
	if (&pOtherLight == this)
		return *this;
	mBlockIsVisible = std::move(pOtherLight.mBlockIsVisible);
	mIntensity = std::move(pOtherLight.mIntensity);
	mColorLight = std::move(pOtherLight.mColorLight);
	mBlockLight = std::move(pOtherLight.mBlockLight);
	return *this;
}

void Light::init(float pIntensity, const glm::vec3& pColorLight, bool pCreateBlockSourceLight)
{
	mIntensity = pIntensity;
	mColorLight = pColorLight;
	mBlockIsVisible = pCreateBlockSourceLight;
	if (pCreateBlockSourceLight)
	{
		std::shared_ptr<Primitive> cube = std::make_unique<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		mBlockLight = std::make_unique<Mesh>(std::weak_ptr<Primitive>(cube));
	}
}

void Light::setIntensity(float pIntensity)
{
	mIntensity = pIntensity;
}

void Light::setColor(const glm::vec3& pColor)
{
	mColorLight = pColor;
}

void Light::setVisibilityForBlockSource(bool pVisibility)
{
	mBlockIsVisible = pVisibility;
}

float Light::getIntensity() const noexcept
{
	return mIntensity;
}

glm::vec3 Light::getColor() const noexcept
{
	return mColorLight;
}

Mesh& Light::getBlockLight()
{
	return *mBlockLight;
}

PointLight::PointLight(const glm::vec3& pPosLight, bool pCreateBlockSourceLight, float pIntensity,
					   const glm::vec3& pColorLight, float pConstant,
					   float pLinear, float pQuadratic)
	: Light(pIntensity, pColorLight, pCreateBlockSourceLight)
{
	init(pPosLight, pCreateBlockSourceLight, pIntensity, pColorLight, pConstant, pLinear, pQuadratic);
}

void PointLight::init(const glm::vec3& pPosLight, bool pCreateBlockSourceLight, float pIntensity,
					  const glm::vec3& pColorLight, float pConstant,
					  float pLinear, float pQuadratic)
{
	mPosLight  = pPosLight;
	mConstant  = pConstant;
	mLinear    = pLinear;
	mQuadratic = pQuadratic;
}

void PointLight::sendToShader(Shader& pShader)
{
	pShader.setUniform3fv("point_light.position", mPosLight);
	pShader.setUniform3fv("point_light.color", getColor());
	pShader.setUniform1f("point_light.intensity", getIntensity());
	pShader.setUniform1f("point_light.constant", mConstant);
	pShader.setUniform1f("point_light.linear", mLinear);
	pShader.setUniform1f("point_light.quadratic", mQuadratic);
}

void PointLight::sendToShaderArray(Shader& pShader, int32_t pIndex)
{
	std::string point_lightArr = "point_light[" + std::to_string(pIndex) + ']';
	pShader.setUniform3fv((point_lightArr + ".position"), mPosLight);
	pShader.setUniform3fv((point_lightArr + ".color"), getColor());
	pShader.setUniform1f((point_lightArr + ".intensity"), getIntensity());
	pShader.setUniform1f((point_lightArr + ".constant"), mConstant);
	pShader.setUniform1f((point_lightArr + ".linear"), mLinear);
	pShader.setUniform1f((point_lightArr  + ".quadratic"), mQuadratic);
}

void PointLight::setPosLight(const glm::vec3& pPosLight)
{
	mPosLight = pPosLight;
}

glm::vec3 PointLight::getPosLight() const noexcept
{
	return mPosLight;
}
