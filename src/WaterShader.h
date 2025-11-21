#pragma once
#include "Shader.h"

class WaterShader : public Shader
{
public:
	WaterShader() = default;
	WaterShader(const std::filesystem::path& pVertPath,
				const std::filesystem::path& pFragPath);

	void init(const std::filesystem::path& pVertPath,
			  const std::filesystem::path& pFragPath);

	void setProjectionMatrix(std::string_view pUniformName, const glm::mat4& pProjMatrix);
	void setViewMatrix(std::string_view pUniformName, const glm::mat4& pViewMatrix);
	void setModelMatrix(std::string_view pUniformName, const glm::mat4& pModelMatrix);
};

