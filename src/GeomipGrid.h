#pragma once
#include <iostream>
#include <format>
#include <vector>
#include <memory>

#include "glm/glm.hpp"

#include "VBO.h"
#include "VAO.h"
#include "VBOLayout.h"
#include "EBO.h"
#include "Light.h"
#include "LodManager.h"
#include "FrustumCulling.h"
#include "Camera.h"
#include "Volume.h"

class Terrain;

class GeomipGrid
{
public:
	GeomipGrid() = default;
	GeomipGrid(int32_t pWidth, int32_t pDepth, uint32_t pPatchSize, 
			   float pDistanceOfChanks, Terrain* pTerrain);
	~GeomipGrid();

	void createGeomipGrid(int32_t pWidth, int32_t pDepth, uint32_t pPatchSize, 
						  float pDistanceOfChanks, Terrain* pTerrain);
	void createGLState();

	void render(Camera* pCamera);
	void destroy();

private:
	void initVertices(const Terrain* pTerrain);
	int32_t initIndices();
	int32_t initIndicesLOD(uint32_t pIndex, int32_t pLod);
	int32_t initIndicesLODSingle(uint32_t pIndex, int32_t pLodCore, int32_t pLodLeft, 
								 int32_t pLodRight, int32_t pLodTop, int32_t pLodBottom);
	void calcNormals(std::vector<Vertex>& pVertices, std::vector<uint32_t>& pIndices);
	void initAABB();

	uint32_t addTriangle(uint32_t pIndex, const std::vector<uint32_t>& pIndices, uint32_t pV1, uint32_t pV2, uint32_t pV3);
	uint32_t createTriangleFan(uint32_t pIndex,	int32_t pLodCore, int32_t pLodLeft, int32_t pLodRight,
							   int32_t pLodTop, int32_t pLodBottom, int32_t pX, int32_t pZ);
	int32_t calcNumIndices();

	bool isPatchInsideFrustum_ViewSpace(int32_t pX, int32_t pZ, const glm::mat4& pViewProj);
	bool isPatchInsideFrustum_WorldSpace(int32_t pX, int32_t pZ, const FrustumCulling& pFrustumCulling);
	bool isCameraInPatch(const glm::vec3& pCameraPos, int32_t pX, int32_t pZ);

private:
	int32_t mWidth{ 0 }, mDepth{ 0 };
	uint32_t mPatchSize{ 0 };
	uint32_t mMaxLod{ 0 };
	float mWorldScale;

	Terrain* mTerrain;

	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndices;

	std::unique_ptr<AABB> mAABB;

	VBO mVBO;
	VBOLayout mVBOLayout;
	VAO mVAO;
	EBO mEBO;

	struct SingleLodInfo
	{
		uint32_t mStart{ 0 };
		uint32_t mCount{ 0 };
	};
#define LEFT_PATCH 2
#define RIGHT_PATCH 2
#define TOP_PATCH 2
#define BOTTOM_PATCH 2
	struct LodInfo
	{
		SingleLodInfo mSingleLodInfo[LEFT_PATCH][RIGHT_PATCH][TOP_PATCH][BOTTOM_PATCH];
	};

	std::vector<LodInfo> mLodInfoStorage;
	uint32_t mNumPatchesX{ 0 };
	uint32_t mNumPatchesZ{ 0 };
	LodManager mLodManager;
};


