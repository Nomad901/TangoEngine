#pragma once
#include <iostream>
#include <vector>
#include "string"
#include <algorithm>

#include "glm/glm.hpp"

#include "Mesh.h"
#include "Shader.h"
#include "TextureManager.h"
#include "HeightsGenerator.h"
#include "Utils.h"
#include "TriangleList.h"

class Terrain
{
public:
	Terrain() = default;
	Terrain(float pWorldScale);

	void init(float pWorldScale);
	void loadFromFile(const std::filesystem::path& pPath);

	void render(const glm::mat4& pViewMat, const glm::mat4& pProj);

	float getHeight(int32_t pX, int32_t pZ) const;
	float getWorldScale() const noexcept;
private:
	void loadHeightMapFile(const std::filesystem::path& pPath);
	
protected:
	void getMinMax(float& pMin, float& pMax);
	void normalize(float pMinRange, float pMaxRange);

protected:
	int32_t mTerrainSize{ 0 };
	float mWorldScale{ 1.0f };
	float mMinHeight{ 0.0f }, mMaxHeight{ 0.0f };
	std::vector<std::vector<float>> mHeightMap;

	Shader mShader;
	TriangleList mTriangleList;
};