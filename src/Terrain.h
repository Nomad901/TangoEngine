#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <span>
#include <algorithm>
#include <memory>

#include "glm/glm.hpp"

#include "Mesh.h"
#include "Shader.h"
#include "TextureManager.h"
#include "Utils.h"
#include "TriangleList.h"
#include "Light.h"

class Terrain
{
public:
	Terrain() = default;
	Terrain(float pWorldScale, float pTexScale, std::span<std::filesystem::path> pPaths);
	Terrain(float pWorldScale, float pTexScale);

	void init(float pWorldScale, float pTexScale, std::span<std::filesystem::path> pPaths);
	void init(float pWorldScale, float pTexScale);
	void loadFromFile(const std::filesystem::path& pPath);
	void setHeights(float pHeight0, float pHeight1, float pHeight2, float pHeight3);
	void setPos(const glm::vec3& pPos);
	void setLight(const glm::vec3& pDirection, float pSoftness);
	void setMinMaxHeight(float pMinHeight, float pMaxHeight);
	void finalizeTerrain();
	
	void render(const glm::mat4& pViewMat, const glm::mat4& pProj, 
				const glm::vec3& pLightPos);

	float getHeight(int32_t pX, int32_t pZ) const;
	float getHeightInterpolated(float pX, float pZ) const;
	float getWorldScale() const noexcept;
	float getTextureScale() const noexcept;
	float getSlopeLight(int32_t pX, int32_t pZ) const noexcept;
	int32_t getTerrainSize() const noexcept;
	

private:
	void loadHeightMapFile(const std::filesystem::path& pPath);
	
protected:
	void getMinMax(float& pMin, float& pMax);
	void normalize(float pMinRange, float pMaxRange);

protected:
	bool mIsOneTex{ true };
	int32_t mTerrainSize{ 0 };
	float mWorldScale{ 1.0f };
	float mTexScale{ 1.0f };
	float mMinHeight{ 0.0f }, mMaxHeight{ 0.0f };

	glm::vec3 mPos{ 1.0f };
	std::vector<std::vector<float>> mHeightMap;
	std::array<std::unique_ptr<Texture2>, 4> mTextures;
	std::array<float, 4> mHeights;

	Shader mShader;
	TriangleList mTriangleList;
	SlopeLight mSlopeLight;
};