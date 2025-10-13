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
#include "LodManager.h"

class Terrain;

class GeomipGrid
{
public:
	GeomipGrid() = default;
	GeomipGrid(int32_t pWidth, int32_t pDepth, uint32_t pPatchSize, Terrain* pTerrain);
	~GeomipGrid();

	void createGeomipGrid(int32_t pWidth, int32_t pDepth, uint32_t pPatchSize, Terrain* pTerrain);
	void createGLState();

	void render(const glm::vec3& pCameraPos);
	void destroy();

private:
	void initVertices(const Terrain* pTerrain);
	int32_t initIndices();
	int32_t initIndicesLOD(uint32_t pIndex, int32_t pLod);
	int32_t initIndicesLODSingle(uint32_t pIndex, int32_t pLodCore, int32_t pLodLeft, 
								 int32_t pLodRight, int32_t pLodTop, int32_t pLodBottom);
	void calcNormals(std::vector<Vertex>& pVertices, std::vector<uint32_t>& pIndices);

	uint32_t addTriangle(uint32_t pIndex, const std::vector<uint32_t>& pIndices, uint32_t pV1, uint32_t pV2, uint32_t pV3);
	uint32_t createTriangleFan(uint32_t pIndex,	int32_t pLodCore, int32_t pLodLeft, int32_t pLodRight,
							   int32_t pLodTop, int32_t pLodBottom, int32_t pX, int32_t pZ);
	int32_t calcNumIndices();

private:
	int32_t mWidth{ 0 }, mDepth{ 0 };
	uint32_t mPatchSize{ 0 };
	uint32_t mMaxLod{ 0 };
	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndices;

	VBO mVBO;
	VBOLayout mVBOLayout;
	VAO mVAO;
	EBO mEBO;

	struct SingleLodInfo
	{
		uint32_t mStart{ 0 };
		uint32_t mCount{ 0 };
	};
#define LEFT 2
#define RIGHT 2
#define TOP 2
#define BOTTOM 2
	struct LodInfo
	{
		SingleLodInfo mSingleLodInfo[LEFT][RIGHT][TOP][BOTTOM];
	};

	std::vector<LodInfo> mLodInfoStorage;
	uint32_t mNumPatchesX{ 0 };
	uint32_t mNumPatchesZ{ 0 };
	LodManager mLodManager;
};


