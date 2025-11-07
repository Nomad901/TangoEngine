#pragma once
#include <memory>

#include "Material.h"
#include "Texture2.h"

struct PBRMaterial : public Material
{
	enum class TEXTURE_TYPE : uint32_t;
public:
	PBRMaterial() = default;
	~PBRMaterial();

	uint32_t getIndex(TEXTURE_TYPE pTEXTURE_TYPE);

public:
	//
	// was trying to repeat blender setup :)
	//
	enum class TEXTURE_TYPE : uint32_t
	{
		TEX_TYPE_BASE = 0,
		TEX_TYPE_METALLIC = 1,
		TEX_TYPE_ROUGHNESS = 2,
		TEX_TYPE_ALPHA = 3,

		TEX_TYPE_NORMAL = 4,
		TEX_TYPE_SPECULAR = 5,

		TEX_TYPE_CLEARCOAT = 6,
		TEX_TYPE_CLEARCOAT_ROUGNESS = 7,
		TEX_TYPE_CLEARCOAT_NORMAL = 8,

		TEX_TYPE_EMISSIVE_COLOR = 9,
		TEX_TYPE_EMISSIVE = 10,

		TEX_TYPE_NORMAL_CAMERA = 11,
		TEX_TYPE_AMBIENT_OCCLUSION = 12,

		NUMBER_OF_TEX_TYPE = 13
	};

	// PBR material parameters;
	glm::vec3 mColor{ glm::vec3(1.0f, 1.0f, 1.0f) };
	bool mIsMetallic{ false };
	float mAlpha{};
	std::unique_ptr<Texture2> mNormalMap;
	std::unique_ptr<Texture2> mSpecularMap;
	std::unique_ptr<Texture2> mRoughness;
	std::unique_ptr<Texture2> mMetallic;
	std::unique_ptr<Texture2> mAO;
	std::unique_ptr<Texture2> mEmissive;

	// General material parameters;
	std::string mName;
	glm::vec4 mAmbientColor{ glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };
	glm::vec4 mDiffuseColor{ glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };
	glm::vec4 mSpecularColor{ glm::vec4(0.0f,0.0f,0.0f,0.0f) };
	glm::vec4 mBaseColor{ glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };
	glm::vec4 mEmissiveColor{ glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };
	glm::vec4 mMetallicRoughnessOcclusion{ glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };
	glm::vec4 mClearCoatTransmission{ glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };
	
	Texture2* mTextures[static_cast<uint32_t>(TEXTURE_TYPE::NUMBER_OF_TEX_TYPE)] = { 0 };

	float mTransparencyFactor{ 1.0f };
	float mAlphaFactor{ 0.0f };
	uint32_t mFlags{ 0 };
};

