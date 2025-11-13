#pragma once
#include <memory>

#include "Material.h"

struct PBRMaterial : public Material
{
	glm::vec3 mColor{ glm::vec3(1.0f, 1.0f, 1.0f) };
	bool mIsMetallic{ false };
	float mAlpha{};
	std::unique_ptr<Texture2> mNormalMap;
	std::unique_ptr<Texture2> mSpecularMap;
	std::unique_ptr<Texture2> mRoughness;
	std::unique_ptr<Texture2> mMetallic;
	std::unique_ptr<Texture2> mAO;
	std::unique_ptr<Texture2> mEmissive;
};

