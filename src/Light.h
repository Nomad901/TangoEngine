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
public:
	Light() = default;
	Light(float pIntensity, const glm::vec3& pColorLight);
	~Light() = default;
	Light(const Light& pOtherLight) = delete;
	Light& operator=(const Light& pOtherLight) = delete;
	Light(Light&& pOtherLight) noexcept;
	Light& operator=(Light&& pOtherLight) noexcept;

	void init(float pIntensity, const glm::vec3& pColorLight);

	virtual void sendToShader(Shader& pShader) = 0;
	virtual void sendToShaderArray(Shader& pShader, int32_t pIndex) = 0;
	
	virtual void setPosLight(const glm::vec3& pPosLight) = 0;
	virtual glm::vec3 getPosLight() const noexcept = 0;

	void enableAttenuation(float pConstant, float pLinear, float pQuadratic);
	void disableAttenuation();
	bool attenuationIsEnabled() const noexcept;

	void setIntensity(float pIntensity);
	void setColor(const glm::vec3& pColor);

	float getIntensity() const noexcept;
	glm::vec3 getColor() const noexcept;

private:
	bool mBlockIsVisible{};
	float mIntensity{};
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

	void sendToShader(Shader& pShader) override;
	void sendToShaderArray(Shader& pShader, int32_t pIndex) override;

	void setPathForShader(const std::filesystem::path& pFragmentPath, 
						  const std::filesystem::path& pVertexPath);

	void setDirection(const glm::vec3& pDirection);
	glm::vec3 getDirection() const noexcept;

private:
	glm::vec3 mPos{ 1.0f };
	glm::vec3 mDirection{ 1.0f };

	Shader mDirectionalLightShader;
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

	void sendToShader(Shader& pShader) override;
	void sendToShaderArray(Shader& pShader, int32_t pIndex) override;

	void setPosLight(const glm::vec3& pPosLight) override;
	glm::vec3 getPosLight() const noexcept override;

	void setPathForShader(const std::filesystem::path& pFragmentPath,
						  const std::filesystem::path& pVertexPath);

private:
	glm::vec3 mPosLight{ 1.0f };

	Shader mPointLightShader;
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

	void setDirection(const glm::vec3& pDirection);
	glm::vec3 getDirection() const noexcept;

	void sendToShader(Shader& pShader) override;
	void sendToShaderArray(Shader& pShader, int32_t pIndex) override;

	void setPosLight(const glm::vec3& pPosLight) override;
	glm::vec3 getPosLight() const noexcept override;

private:
	float mRadius{ 1.0f };

	glm::vec3 mPosLight{ 1.0f };
	glm::vec3 mDirection{ 1.0f };

	Shader mSpotLightShader;
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

	void setPathForShader(const std::filesystem::path& pFragmentPath,
						  const std::filesystem::path& pVertexPath);

	void setOuterCos(float pOuterCos);
	void setInnerCos(float pInnerCos);
	float getOuterCos() const noexcept;
	float getInnerCos() const noexcept;

	void sendToShader(Shader& pShader) override;
	void sendToShaderArray(Shader& pShader, int32_t pIndex) override;

	void setPosLight(const glm::vec3& pPosLight) override;
	glm::vec3 getPosLight() const noexcept override;

private:
	glm::vec3 mPos{ 1.0f };
	glm::vec3 mDirection{ 1.0f };

	Shader mFlashlightShader;

	float outerCutOff, innerCutOff;
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


	void setPathForShader(const std::filesystem::path& pFragmentPath,
						  const std::filesystem::path& pVertexPath);

	void sendToShader(Shader& pShader) override;
	void sendToShaderArray(Shader& pShader, int32_t pIndex) override;

	void setPosLight(const glm::vec3& pPosLight) override;
	glm::vec3 getPosLight() const noexcept override;

private:
	glm::vec3 mPos{ 1.0f };
	
	Shader mAreaLight;
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

	void sendToShader(Shader& pShader) override;
	void sendToShaderArray(Shader& pShader, int32_t pIndex) override;

	void setPosLight(const glm::vec3& pPosLight) override;
	glm::vec3 getPosLight() const noexcept override;

private:
	bool mIsFlashing{ false };

	glm::vec3 mPos{ 1.0f };

	Shader mEmissiveLightShader;
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

	void sendToShader(Shader& pShader) override;
	void sendToShaderArray(Shader& pShader, int32_t pIndex) override;

	void setPosLight(const glm::vec3& pPosLight) override;
	glm::vec3 getPosLight() const noexcept override;

private:
	glm::vec3 mPos{ 1.0f };

	Shader mAttenuationLightShader;
};
