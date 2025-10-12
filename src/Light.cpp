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

void SlopeLight::init(const std::vector<std::vector<float>>& pHeightMap, const glm::vec3& pLightDir, 
					  uint32_t pTerrainSize, float pSoftness)
{
	mHeightMap = pHeightMap;
	mTerrainSize = pTerrainSize;
	mSoftness = pSoftness;

	glm::vec3 reversedLightDir = pLightDir * -1.0f;
	reversedLightDir.y = 0.0f;
	reversedLightDir = glm::normalize(reversedLightDir);

	glm::vec3 dirX(1.0f, 0.0f, 0.0f);
	float dpx = glm::dot(dirX, reversedLightDir);

	glm::vec3 dirZ(0.0f, 0.0f, 1.0f);
	float dpz = glm::dot(dirZ, reversedLightDir);

	float radiansTo45Degrees = cosf(glm::radians(45.0f));

	bool interpolatedOnX = false;

	if (dpz >= radiansTo45Degrees)
	{
		mDZ0 = mDZ1 = 1;
		interpolatedOnX = true;
	}
	else if (dpz <= -radiansTo45Degrees)
	{
		mDZ0 = mDZ1 = -1;
		interpolatedOnX = true;
	}
	else
	{
		if (dpz >= 0.0f)
		{
			mDZ0 = 0;
			mDZ1 = 1;
		}
		else
		{
			mDZ0 = 0;
			mDZ1 = -1;
		}
		mFactor = 1.0f - abs(dpz) / radiansTo45Degrees;

		if (dpx >= 0.0f)
		{
			mDX0 = mDX1 = 1;
		}
		else 
		{
			mDX0 = mDX1 = -1;
		}
	}

	if (interpolatedOnX)
	{
		if (dpx >= 0.0f)
		{
			mDX0 = 0;
			mDX1 = 1;
		}
		else
		{
			mDX0 = 0;
			mDX1 = -1;
		}

		mFactor = 1.0f - abs(dpx) / radiansTo45Degrees;
	}
}

float SlopeLight::getBrightness(int32_t pX, int32_t pZ) const
{
	float height = mHeightMap[pX][pZ];
	float brightness = 0.0f;

	float XBefore0 = pX + mDX0 * 5.0f;
	float ZBefore0 = pZ + mDZ0 * 5.0f;

	float XBefore1 = pX + mDX1 * 5.0f;
	float ZBefore1 = pZ + mDZ1 * 5.0f;

	bool V0InsideHeightMap = ((XBefore0 > 0.0f) && (XBefore0 < mTerrainSize)) && ((ZBefore0 > 0.0f) && (ZBefore0 < mTerrainSize));
	bool V1InsideHeightMap = ((XBefore1 > 0.0f) && (XBefore1 < mTerrainSize)) && ((ZBefore1 > 0.0f) && (ZBefore1 < mTerrainSize));

	float minBrightness = 0.4f;
	
	if (V0InsideHeightMap && V1InsideHeightMap)
	{
		float heightBefore0 = mHeightMap[XBefore0][ZBefore0];
		float heightBefore1 = mHeightMap[XBefore1][ZBefore1];

		float heightBefore = heightBefore0 * mFactor + (1.0f - mFactor) * heightBefore1;
		brightness = (height - heightBefore) / mSoftness;
	}
	else if (V0InsideHeightMap)
	{
		float heightBefore = mHeightMap[XBefore0][ZBefore0];
		brightness = (height - heightBefore) / mSoftness;
	}
	else if (V1InsideHeightMap)
	{
		float heightBefore = mHeightMap[XBefore1][ZBefore1];
		brightness = (height - heightBefore) / mSoftness;
	}
	else
	{
		brightness = 1.0f;
	}
	
	brightness = std::min(1.0f, std::max(brightness, minBrightness));

	return brightness;
}

void SlopeLight::setPosLight(const glm::vec3& pPosLight)
{
	mPos = pPosLight;
}

glm::vec3 SlopeLight::getPosLight() const noexcept
{
	return mPos;
}

void SlopeLight::setDirectionLight(const glm::vec3& pDirectionLight)
{
	mDirection = pDirectionLight;
}

glm::vec3 SlopeLight::getDirectionLight() const noexcept
{
	return mDirection;
}

void SlopeLight::setSoftness(float pSoftness)
{
	mSoftness = pSoftness;
}

float SlopeLight::getSoftness() const noexcept
{
	return mSoftness;
}
