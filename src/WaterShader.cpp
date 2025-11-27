#include "WaterShader.h"
#include "WaterFBO.h"
#include "Texture2.h"

WaterShader::WaterShader(const std::filesystem::path& pVertPath, const std::filesystem::path& pFragPath)
{
	init(pVertPath, pFragPath);
}

void WaterShader::init(const std::filesystem::path& pVertPath, const std::filesystem::path& pFragPath)
{
	Shader::init(pVertPath, pFragPath);
}

void WaterShader::setProjectionMatrix(std::string_view pUniformName, const glm::mat4& pProjMatrix)
{
	Shader::setMatrixUniform4fv(pUniformName, pProjMatrix);
}

void WaterShader::setViewMatrix(std::string_view pUniformName, const glm::mat4& pViewMatrix)
{
	Shader::setMatrixUniform4fv(pUniformName, pViewMatrix);
}

void WaterShader::setModelMatrix(std::string_view pUniformName, const glm::mat4& pModelMatrix)
{
	Shader::setMatrixUniform4fv(pUniformName, pModelMatrix);
}

void WaterShader::setReflectionTexture(std::string_view pUniformName, WaterFBO& pWaterFBO)
{
	uint32_t indexOfReflectionTexture = getIndex(indicesOfTextures::REFLECTION_TEXTURE);
	pWaterFBO.getReflectionTexture().bind(indexOfReflectionTexture);
	Shader::setUniform1i(pUniformName, indexOfReflectionTexture);
}

void WaterShader::setRefractionTexture(std::string_view pUniformName, WaterFBO& pWaterFBO)
{
	uint32_t indexOfRefractionTexture = getIndex(indicesOfTextures::REFRACTION_TEXTURE);
	pWaterFBO.getReflectionTexture().bind(indexOfRefractionTexture);
	Shader::setUniform1i(pUniformName, indexOfRefractionTexture);
}

void WaterShader::setDuDvMap(std::string_view pUniformName, Texture2& pDuDvMap)
{
	uint32_t indexOfDuDvMap = getIndex(indicesOfTextures::DUDV_MAP);
	pDuDvMap.bind(indexOfDuDvMap);
	Shader::setUniform1i(pUniformName, indexOfDuDvMap);
}

void WaterShader::setNormalMap(std::string_view pUniformName, Texture2& pNormalMap)
{
	uint32_t indexOfNormalMap = getIndex(indicesOfTextures::NORMAL_MAP);
	pNormalMap.bind(indexOfNormalMap);
	Shader::setUniform1i(pUniformName, indexOfNormalMap);
}

void WaterShader::setDepthMap(std::string_view pUniforName, Texture2& pDepthMap)
{
	uint32_t indexOfDepthMap = getIndex(indicesOfTextures::DEPTH_MAP);
	pDepthMap.bind(indexOfDepthMap);
	Shader::setUniform1i(pUniforName, indexOfDepthMap);
}

void WaterShader::setMoveFactor(std::string_view pUniformName, float pMoveFactor)
{
	Shader::setUniform1f(pUniformName, pMoveFactor);
}

void WaterShader::setLightPos(std::string_view pUniformName, const glm::vec3& pLightPos)
{
	Shader::setUniform3fv(pUniformName, pLightPos);
}

void WaterShader::setLightColor(std::string_view pUniformName, const glm::vec3& pLightColor)
{
	Shader::setUniform3fv(pUniformName, pLightColor);
}

void WaterShader::setNearPlane(std::string_view pUniformName, float pNearPlane)
{
	Shader::setUniform1f(pUniformName, pNearPlane);
}

void WaterShader::setFarPlane(std::string_view pUniformName, float pFarPlane)
{
	Shader::setUniform1f(pUniformName, pFarPlane);
}

uint32_t WaterShader::getIndex(indicesOfTextures pIndicesOfTextures)
{
	return static_cast<uint32_t>(pIndicesOfTextures);
}
