#pragma once
#include "Shader.h"

class WaterFBO;
class Texture2;

class WaterShader : public Shader
{
public:
	enum class indicesOfTextures : uint32_t
	{
		REFLECTION_TEXTURE = 0,
		REFRACTION_TEXTURE = 1,
		DUDV_MAP = 2,
		NORMAL_MAP = 3
	};
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
	void setNormalMap(std::string_view pUniformName, Texture2& pNormalMap);
	void setMoveFactor(std::string_view pUniformName, float pMoveFactor);

private:
	uint32_t getIndex(indicesOfTextures pIndicesOfTextures);
};

