#pragma once
#include "Shader.h"

class WaterFBO;
class Texture2;

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
	void setReflectionTexture(std::string_view pUniformName, WaterFBO& pWaterFBO);
	void setRefractionTexture(std::string_view pUniformName, WaterFBO& pWaterFBO);
	void setDuDvMap(std::string_view pUniformName, Texture2& pDuDvMap);
	void setMoveFactor(std::string_view pUniformName, float pMoveFactor);
};

