#include "WaterShader.h"

void WaterShader::init()
{
	std::string resourcePath = RESOURCES_PATH;
	Shader::init(resourcePath + "Shaders/waterVert.glsl", resourcePath + "Shaders/waterFrag.glsl");
}

void WaterShader::setVP(const glm::mat4& pVP)
{
    Shader::setMatrixUniform4fv("uVP", pVP);
}

void WaterShader::setReflectionTexUnit(uint32_t pTexUnit)
{
    Shader::setUniform1i("uReflectionTexture", pTexUnit);
}

void WaterShader::setRefractionTexUnit(uint32_t pTexUnit)
{
    Shader::setUniform1i("uRefractionTexture", pTexUnit);
}

void WaterShader::setDUDVMapTexUnit(uint32_t pTexUnit)
{
    Shader::setUniform1i("uDUDVMapTexture", pTexUnit);
}

void WaterShader::setNormalMapTexUnit(uint32_t pTexUnit)
{
    Shader::setUniform1i("uNormalMap", pTexUnit);
}

void WaterShader::setDepthMapTexUnit(uint32_t pTexUnit)
{
    Shader::setUniform1i("uDepthMap", pTexUnit);
}

void WaterShader::setWaterHeight(float pHeight)
{
    Shader::setUniform1f("uHeight", pHeight);
}

void WaterShader::setDUDVOffset(float pOffset)
{
    Shader::setUniform1f("vDUDVOffset", pOffset);
}

void WaterShader::setCameraPos(const glm::vec3& pCameraPos)
{
    Shader::setUniform3fv("uCameraPos", pCameraPos);
}

void WaterShader::setLightColor(const glm::vec4& pLightColor)
{
    Shader::setUniform4fv("uLightColor", pLightColor);
}

void WaterShader::setLightDir(const glm::vec3& pLightDir)
{
    Shader::setUniform3fv("uReversedLightDir", pLightDir);
}