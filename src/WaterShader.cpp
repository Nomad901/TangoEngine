#include "WaterShader.h"

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
