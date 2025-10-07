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
	Terrain();

	void loadFromFile(const std::filesystem::path& pPath);
	float getHeight(int32_t pX, int32_t pZ) const;

	void render(const glm::mat4& pViewMat);

private:
	void loadHeightMapFile(const std::filesystem::path& pPath);

private:
	int32_t mTerrainSize{ 0 };
	std::vector<std::vector<float>> mHeightMap;

	Shader mShader;
	TriangleList mTriangleList;
};