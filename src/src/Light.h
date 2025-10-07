#pragma once
#include <iostream>
#include <format>

#include "glm.hpp"
#include "SDL3/SDL.h"
#include "glad/glad.h"

#include "Mesh.h"
#include "Shader.h"

class Light
{
private:
	struct Attenuation;
public:
	Light() = default;
	Light(float pIntensity, const glm::vec3& pColorLight);
	~Light() = default;
	Light(const Light& pOtherLight) = delete;
	Light& operator=(const Light& pOtherLight) = delete;
	Light(Light&& pOtherLight) noexcept;
	Light& operator=(Light&& pOtherLight) noexcept;

	void init(float pIntensity, const glm::vec3& pColorLight);

	virtual void setPosLight(const glm::vec3& pPosLight) = 0;
	virtual glm::vec3 getPosLight() const noexcept = 0;
	virtual void setDirectionLight(const glm::vec3& pDirectionLight) = 0;
	virtual glm::vec3 getDirectionLight() const noexcept = 0;

	void enableAttenuation(float pConstant, float pLinear, float pQuadratic);
	const Attenuation getAttenuationSettings() const noexcept;
	void disableAttenuation();
	bool attenuationIsEnabled() const noexcept;

	void setAmbient(const glm::vec3& pAmbient);
	void setDiffuse(const glm::vec3& pDiffuse);
	void setSpecular(const glm::vec3& pSpecular);
	glm::vec3 getAmbient() const noexcept;
	glm::vec3 getDiffuse() const noexcept;
	glm::vec3 getSpecular() const noexcept;

	void setIntensity(float pIntensity);
	void setColor(const glm::vec3& pColor);

	float getIntensity() const noexcept;
	glm::vec3 getColor() const noexcept;

private:
	bool mBlockIsVisible{};
	float mIntensity{};
	glm::vec3 mAmbient{ 0.1f, 0.1f, 0.1f }, mDiffuse{ 0.8f, 0.8f, 0.8f }, mSpecular{ 1.0f, 1.0f, 1.0f };
	glm::vec3 mColorLight{};

	struct Attenuation
	{
		bool mAttenuationIsUp{ false };
		float mConstant{};
		float mLinear{};
		float mQuadratic{};
	} mAttenuation;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight() = default;

	// 
	// without attenuation
	//
	DirectionalLight(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pIntensity = 1.0f,
					 const glm::vec3& pColorLight = glm::vec3(1.0f));
	// 
	// with attenuation
	//
	DirectionalLight(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pConstant, float pLinear, float pQuadratic,
					 float pIntensity = 1.0f,
					 const glm::vec3& pColorLight = glm::vec3(1.0f));

	// 
	// without attenuation
	//
	void init(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pIntensity = 1.0f,
			  const glm::vec3& pColorLight = glm::vec3(1.0f));
	// 
	// with attenuation
	//
	void init(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pConstant, float pLinear, float pQuadratic,
			  float pIntensity = 1.0f,
			  const glm::vec3& pColorLight = glm::vec3(1.0f));

	void setPosLight(const glm::vec3& pPos) override;
	glm::vec3 getPosLight() const noexcept override;
	void setDirectionLight(const glm::vec3& pDirectionLight) override;
	glm::vec3 getDirectionLight() const noexcept override;

	void setDirection(const glm::vec3& pDirection);
	glm::vec3 getDirection() const noexcept;

private:
	glm::vec3 mPos{ 1.0f };
	glm::vec3 mDirection{ 1.0f };
};

class PointLight : public Light
{
public:
	PointLight() = default;

	// 
	// without attenuation
	//
	PointLight(const glm::vec3& pPosLight, float pIntensity = 1.0f,
			   const glm::vec3& pColorLight = glm::vec3(1.0f));
	// 
	// with attenuation
	//
	PointLight(const glm::vec3& pPosLight, float pConstant, float pLinear, float pQuadratic,
			   float pIntensity = 1.0f,
			   const glm::vec3& pColorLight = glm::vec3(1.0f));

	// 
	// without attenuation
	//
	void init(const glm::vec3& pPosLight, float pIntensity = 1.0f,
			  const glm::vec3& pColorLight = glm::vec3(1.0f));
	// 
	// with attenuation
	//
	void init(const glm::vec3& pPosLight, float pConstant, float pLinear, float pQuadratic,
			  float pIntensity = 1.0f,
		  	  const glm::vec3& pColorLight = glm::vec3(1.0f));

	void setPosLight(const glm::vec3& pPosLight) override;
	glm::vec3 getPosLight() const noexcept override;
	void setDirectionLight(const glm::vec3& pDirectionLight) override;
	glm::vec3 getDirectionLight() const noexcept override;

private:
	glm::vec3 mPosLight{ 1.0f };
};

class Spotlight : public Light
{
public:
	Spotlight() = default;

	// 
	// without attenuation
	//
	Spotlight(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pRadius, float pIntensity = 1.0f,
			  const glm::vec3& pColorLight = glm::vec3(1.0f));
	// 
	// with attenuation
	//
	Spotlight(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pRadius, 
			  float pConstant, float pLinear, float pQuadratic, float pIntensity = 1.0f,
			  const glm::vec3& pColorLight = glm::vec3(1.0f));

	// 
	// without attenuation
	//
	void init(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pRadius, float pIntensity = 1.0f,
			  const glm::vec3& pColorLight = glm::vec3(1.0f));
	// 
	// with attenuation
	//
	void init(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pRadius, 
			  float pConstant, float pLinear, float pQuadratic, float pIntensity = 1.0f,
			  const glm::vec3& pColorLight = glm::vec3(1.0f));

	void setRadius(float pRadius);
	float getRadius() const noexcept;

	void setCutOff(float pCutOff);
	float getCutOff() const noexcept;

	void setDirectionLight(const glm::vec3& pDirectionLight) override;
	glm::vec3 getDirectionLight() const noexcept override;

	void setPosLight(const glm::vec3& pPosLight) override;
	glm::vec3 getPosLight() const noexcept override;

private:
	float CutOff{ 12.5f };

	glm::vec3 mPosLight{ 1.0f };
	glm::vec3 mDirection{ 1.0f };
};

class Flashlight : public Spotlight
{
public:
	Flashlight() = default;

	//
	// u can set attenuation then, via methods in the base class;
	// InnerCos - the radius of the beginning of the flashlight;
	// OuterCos - the opposite, the radius of the end of the flashlight;
	//
	Flashlight(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pInnerCutOff, float pOuterCutOff,
			   float pIntensity = 1.0f,
			   const glm::vec3& pColorLight = glm::vec3(1.0f));
	
	//
	// u can set attenuation then, via methods in the base class;
	//
	void init(const glm::vec3& pPosLight, const glm::vec3& pDirection, float pInnerCutOff, float pOuterCutOff, 
			  float pIntensity = 1.0f,
			  const glm::vec3& pColorLight = glm::vec3(1.0f));

	void setOuterCutOff(float pOuterCutOff);
	void setCutOff(float pCutOff);
	float getOuterCutOff() const noexcept;
	float getCutOff() const noexcept;

	void setPosLight(const glm::vec3& pPosLight) override;
	glm::vec3 getPosLight() const noexcept override;
	
	void setDirectionLight(const glm::vec3& pDirectionLight) override;
	glm::vec3 getDirectionLight() const noexcept override;

private:
	float outerCutOff{ 17.5f }, CutOff{ 12.5f };

	glm::vec3 mPos{ 1.0f };
	glm::vec3 mDirection{ 1.0f };
};

class AreaLight : public Light
{
public:
	AreaLight() = default;

	// 
	// without attenuation
	//
	AreaLight(const glm::vec3& pPosLight, float pIntensity = 1.0f,
			  const glm::vec3& pColorLight = glm::vec3(1.0f));
	// 
	// with attenuation
	//
	AreaLight(const glm::vec3& pPosLight, float pConstant, float pLinear, float pQuadratic,
			  float pIntensity = 1.0f,
			  const glm::vec3& pColorLight = glm::vec3(1.0f));

	// 
	// without attenuation
	//
	void init(const glm::vec3& pPosLight, float pIntensity = 1.0f,
			  const glm::vec3& pColorLight = glm::vec3(1.0f));
	// 
	// with attenuation
	//
	void init(const glm::vec3& pPosLight, float pConstant, float pLinear, float pQuadratic,
			  float pIntensity = 1.0f,
			  const glm::vec3& pColorLight = glm::vec3(1.0f));

	void setPosLight(const glm::vec3& pPosLight) override;
	glm::vec3 getPosLight() const noexcept override;

	void setDirectionLight(const glm::vec3& pDirectionLight) override;
	glm::vec3 getDirectionLight() const noexcept override;

private:
	glm::vec3 mPos{ 1.0f };
};

//
// neon
//
class EmissiveLight : public Light
{
public:
	EmissiveLight() = default;
	EmissiveLight(const glm::vec3& pPosLight, bool pFlashing, float pIntensity = 1.0f,
				  const glm::vec3& pColorLight = glm::vec3(1.0f));
	void init(const glm::vec3& pPosLight, bool pFlashing, float pIntensity = 1.0f,
			  const glm::vec3& pColorLight = glm::vec3(1.0f));

	void setFlashing(bool pFlashing);
	bool isFlashing() const noexcept;

	void setPosLight(const glm::vec3& pPosLight) override;
	glm::vec3 getPosLight() const noexcept override;

private:
	bool mIsFlashing{ false };

	glm::vec3 mPos{ 1.0f };
};

class AttenuationLight : public Light
{
public:
	AttenuationLight() = default;

	AttenuationLight(const glm::vec3& pPosLight, float pConstant, float pLinear, float pQuadratic, 
					 float pIntensity = 1.0f,
					 const glm::vec3& pColorLight = glm::vec3(1.0f));

	void init(const glm::vec3& pPosLight, float pConstant, float pLinear, float pQuadratic, 
			  float pIntensity = 1.0f,
			  const glm::vec3& pColorLight = glm::vec3(1.0f));

	void setPosLight(const glm::vec3& pPosLight) override;
	glm::vec3 getPosLight() const noexcept override;

	void setDirectionLight(const glm::vec3& pDirectionLight) override;
	glm::vec3 getDirectionLight() const noexcept override;

private:
	glm::vec3 mPos{ 1.0f };
};
