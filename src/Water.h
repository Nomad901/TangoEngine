#pragma once
#include "Texture2.h"
#include "FBO.h"
#include "TriangleList.h"
#include "WaterShader.h"

class Water
{
public:
	Water();
	~Water() = default;

	void init(int32_t pSize, float pWorldSize);
	
	void setWorldHeight(float pHeight);

	void render(const glm::vec3& pCamPos, const glm::mat4& pVP, const glm::vec3& pLightDir);

	//
	// reflection
	//
	void startReflectionPass();
	void stopReflectionPass();
	//
	// refraction
	//
	void startRefractionPass();
	void stopRefractionPass();

	float getHeight() const noexcept;
	GLint getReflectionFBO() const noexcept;
	GLint getRefractionFBO() const noexcept;
	GLint getDudvMapTexture() const noexcept;

private:
	float mHeight{ 0.0f };

	WaterShader mWaterShader;
	TriangleList mTriangleList;
	FBO mReflectionFBO;
	FBO mRefractionFBO;
	Texture2 mDudvMap;
	Texture2 mNormalMap;
};

