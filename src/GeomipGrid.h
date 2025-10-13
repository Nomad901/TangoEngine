#pragma once
#include <iostream>
#include <format>
#include <vector>

#include "glm/glm.hpp"

#include "VBO.h"
#include "VAO.h"
#include "VBOLayout.h"
#include "EBO.h"
#include "Light.h"

class Terrain;

class GeomipGrid
{
public:
	GeomipGrid() = default;
	GeomipGrid(int32_t pWidth, int32_t pDepth, uint32_t pPatchSize, Terrain* pTerrain);
	~GeomipGrid();

	void createGeomipGrid(int32_t pWidth, int32_t pDepth, uint32_t pPatchSize, Terrain* pTerrain);
	void createGLState();

	void render();
	void destroy();

private:
	void initVertices(const Terrain* pTerrain);
	void initIndices();
	void calcNormals(std::vector<Vertex>& pVertices, std::vector<uint32_t>& pIndices);

	uint32_t addTriangle(uint32_t pIndex, const std::vector<uint32_t>& pIndices, uint32_t pV1, uint32_t pV2, uint32_t pV3);

private:
	int32_t mWidth{ 0 }, mDepth{ 0 };
	uint32_t mPatchSize{ 0 };
	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndices;

	VBO mVBO;
	VBOLayout mVBOLayout;
	VAO mVAO;
	EBO mEBO;
};


