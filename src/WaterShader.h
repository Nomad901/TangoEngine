#pragma once
#include "Shader.h"

class WaterShader : public Shader
{
public:
	WaterShader() = default;
	 
	void init();

	void setVP(const glm::mat4& pVP);
	void setReflectionTexUnit(uint32_t pTexUnit);
	void setRefractionTexUnit(uint32_t pTexUnit);
	void setDUDVMapTexUnit(uint32_t pTexUnit);
	void setNormalMapTexUnit(uint32_t pTexUnit);
	void setDepthMapTexUnit(uint32_t pTexUnit);
	void setWaterHeight(float pHeight);
	void setDUDVOffset(float pOffset);
	void setCameraPos(const glm::vec3& pCameraPos);
	void setLightColor(const glm::vec4& pLightColor);
	void setLightDir(const glm::vec3& pLightDir);
};

