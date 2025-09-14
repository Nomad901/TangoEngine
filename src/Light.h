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
	Light(float pIntensity, const glm::vec3& pColorLight, bool pCreateBlockSourceLight);
	~Light() = default;
	Light(const Light& pOtherLight) = delete;
	Light& operator=(const Light& pOtherLight) = delete;
	Light(Light&& pOtherLight) noexcept;
	Light& operator=(Light&& pOtherLight) noexcept;

	void init(float pIntensity, const glm::vec3& pColorLight, bool pCreateBlockSourceLight);

	virtual void sendToShader(Shader& pShader) = 0;
	virtual void sendToShaderArray(Shader& pShader, int32_t pIndex) = 0;

	void setIntensity(float pIntensity);
	void setColor(const glm::vec3& pColor);
	void setVisibilityForBlockSource(bool pVisibility);

	float getIntensity() const noexcept;
	glm::vec3 getColor() const noexcept;
	Mesh& getBlockLight();

private:
	bool mBlockIsVisible{};
	float mIntensity{};
	glm::vec3 mColorLight{};
	std::unique_ptr<Mesh> mBlockLight;

};

class PointLight : public Light
{
public:
	PointLight() = default;
	PointLight(const glm::vec3& pPosLight, bool pCreateBlockSourceLight, float pIntensity = 1.0f,
			   const glm::vec3& pColorLight = glm::vec3(1.0f), float pConstant = 1.0f, 
			   float pLinear = 0.045f, float pQuadratic = 0.075f);

	void init(const glm::vec3& pPosLight, bool pCreateBlockSourceLight, float pIntensity = 1.0f,
			  const glm::vec3& pColorLight = glm::vec3(1.0f), float pConstant = 1.0f,
			  float pLinear = 0.045f, float pQuadratic = 0.075f);
	
	void sendToShader(Shader& pShader) override;
	void sendToShaderArray(Shader& pShader, int32_t pIndex) override;

	void setPosLight(const glm::vec3& pPosLight);
	glm::vec3 getPosLight() const noexcept;

private:
	glm::vec3 mPosLight;
	float mConstant{};
	float mLinear{};
	float mQuadratic{};

};