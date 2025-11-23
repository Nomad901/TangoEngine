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
	pWaterFBO.getReflectionTexture().bind(0);
	Shader::setUniform1i(pUniformName, 0);
}

void WaterShader::setRefractionTexture(std::string_view pUniformName, WaterFBO& pWaterFBO)
{
	pWaterFBO.getReflectionTexture().bind(1);
	Shader::setUniform1i(pUniformName, 1);
}

void WaterShader::setDuDvMap(std::string_view pUniformName, Texture2& pDuDvMap)
{
	pDuDvMap.bind(2);
	Shader::setUniform1i(pUniformName, 2);
}

void WaterShader::setMoveFactor(std::string_view pUniformName, float pMoveFactor)
{
	Shader::setUniform1f(pUniformName, pMoveFactor);
}
