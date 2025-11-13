#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Texture2.h"

struct Material
{
	enum class TEXTURE_TYPE : uint32_t;
public:
	Material() = default;

	uint32_t getIndex(TEXTURE_TYPE pTextureType);

public:
	enum class TEXTURE_TYPE : uint32_t
	{
		TEX_TYPE_BASE = 0,
		TEX_TYPE_SPECULAR = 1,
		TEX_TYPE_NORMAL = 2,
		TEX_TYPE_METALNESS = 3,
		TEX_TYPE_EMISSIVE = 4,
		TEX_TYPE_NORMAL_CAMERA = 5,
		TEX_TYPE_EMISSION_COLOR = 6,
		TEX_TYPE_ROUGHNESS = 7,
		TEX_TYPE_AMBIENT_OCCLUSION = 8,
		TEX_TYPE_CLEARCOAT = 9,
		TEX_TYPE_CLEARCOAT_ROUGHNESS = 10,
		TEX_TYPE_CLEARCOAT_NORMAL = 11,
		TEX_TYPE_NUM = 12
	};
	
public:
	// General material parameters;
	std::string mName;
	glm::vec4 mAmbientColor{ glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };
	glm::vec4 mDiffuseColor{ glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };
	glm::vec4 mSpecularColor{ glm::vec4(0.0f,0.0f,0.0f,0.0f) };
	glm::vec4 mBaseColor{ glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };
	glm::vec4 mEmissiveColor{ glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };
	glm::vec4 mMetallicRoughnessOcclusion{ glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };
	glm::vec4 mClearCoatTransmission{ glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };

	std::array<std::unique_ptr<Texture2>, static_cast<uint32_t>(TEXTURE_TYPE::TEX_TYPE_NUM)> mTextures;

	float mTransparencyFactor{ 1.0f };
	float mAlphaFactor{ 0.0f };
	uint32_t mFlags{ 0 };
};

