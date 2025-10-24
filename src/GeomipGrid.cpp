#include "GeomipGrid.h"
#include "Terrain.h"

GeomipGrid::GeomipGrid(int32_t pWidth, int32_t pDepth, uint32_t pPatchSize, 
					   float pDistanceOfChanks, Terrain* pTerrain)
{
	createGeomipGrid(pWidth, pDepth, pPatchSize, pDistanceOfChanks, pTerrain);
}

GeomipGrid::~GeomipGrid()
{
	destroy();
}

void GeomipGrid::createGeomipGrid(int32_t pWidth, int32_t pDepth, uint32_t pPatchSize, 
								  float pDistanceOfChanks, Terrain* pTerrain)
{
	/*
		Checking if both numbers are odd, cuz it will help us in the future in erasing gaps and appending a "center" vertex
	*/
	if ((pWidth - 1) % (pPatchSize - 1) != 0)
	{
		int32_t recommendedWidth = ((pWidth - 1 + pPatchSize - 1) / (pPatchSize - 1)) * (pPatchSize - 1) + 1;
		std::cout << std::format("Width must be divisible by patchSize. Width and patch size, which uve passed: {} / {}\n", pWidth, pPatchSize);
		std::cout << std::format("Recommended width: {}. This will be set\n", recommendedWidth);
		pWidth = recommendedWidth;
	}
	if ((pDepth - 1) % (pPatchSize - 1) != 0)
	{
		int32_t recommendedDepth = ((pDepth - 1 + pPatchSize - 1) / (pPatchSize - 1)) * (pPatchSize - 1) + 1;
		std::cout << std::format("pDepth must be divisible by patchSize. Depth and patch size, which uve passed: {} / {}\n", pDepth, pPatchSize);
		std::cout << std::format("Recommended pDepth: {}. This will be set\n", recommendedDepth);
		pDepth = recommendedDepth;
	}
	if (pPatchSize < 3)
	{
		std::cout << "Minimum patch size is 3!Current patch size will be 3.\n";
		pPatchSize = 3;
	}
	if (pPatchSize % 2 == 0)
	{
		pPatchSize++;
		std::cout << std::format("Patch size needs to be an odd number! Current patchSize will be: {}\n", pPatchSize);
	}

	mWidth = pWidth;
	mDepth = pDepth;
	mPatchSize = pPatchSize;

	/*
		calculating number of patches along X and Z axis by dividing width and depth on patch size. 
		dividing is happening with even numbers;
	*/
	mNumPatchesX = (pWidth - 1) / (pPatchSize - 1);
	mNumPatchesZ = (pDepth - 1) / (pPatchSize - 1);

	mWorldScale = pTerrain->getWorldScale();
	mMaxLod = mLodManager.initLodManger(pPatchSize, mNumPatchesX, mNumPatchesZ, pDistanceOfChanks, mWorldScale);
	mLodInfoStorage.resize(mMaxLod + 1);
	
	mTerrain = pTerrain;

	if (!mVertices.empty() || !mIndices.empty())
	{
		mVertices.clear();
		mIndices.clear();
	}
	initVertices(pTerrain);
	initIndices();
	calcNormals(mVertices, mIndices);
	createGLState();
	initAABB();
}

void GeomipGrid::createGLState()
{
	destroy();
	mVAO.generate();
	mVAO.bind();
	mVBO.init(mVertices, GL_STATIC_DRAW);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 4);
	mVBOLayout.pushLayout(GL_FLOAT, 2);
	mVBOLayout.pushLayout(GL_FLOAT, 1);
	mVAO.addBuffer(mVBO, mVBOLayout);
	mEBO.init(mIndices.data(), mIndices.size());
}

void GeomipGrid::destroy()
{
	if (mVAO.getID() > 0)
		mVAO.destroy();
	if (mVBO.getID() > 0)
		mVBO.destroy();
	if (mVBOLayout.getCount() != 0)
		mVBOLayout.destroy();
	if (mEBO.getID() > 0)
		mEBO.destroy();
}

void GeomipGrid::initVertices(const Terrain* pTerrain)
{
	mVertices.resize(mDepth * mWidth);
	int32_t index = 0;
	for (int32_t z = 0; z < mDepth; ++z)
	{
		for (int32_t x = 0; x < mWidth; ++x)
		{
			assert(index < mVertices.size());
			mVertices[index].initVertex(pTerrain, x, z);
			index++;
		}
	}
}

int32_t GeomipGrid::initIndices()
{
	int32_t numIndices = calcNumIndices();
	mIndices.resize(numIndices);
	uint32_t index = 0;
	for (int32_t lod = 0; lod <= mMaxLod; ++lod)
	{
		index = initIndicesLOD(index, lod);
	}
	return index;
}

int32_t GeomipGrid::initIndicesLOD(uint32_t pIndex, int32_t pLod)
{
	for (int32_t left = 0; left < LEFT_PATCH; ++left)
	{
		for (int32_t right = 0; right < RIGHT_PATCH; ++right)
		{
			for (int32_t top = 0; top < TOP_PATCH; ++top)
			{
				for (int32_t bottom = 0; bottom < BOTTOM_PATCH; ++bottom)
				{
					// start - is the current index of the lod;
					mLodInfoStorage[pLod].mSingleLodInfo[left][right][top][bottom].mStart = pIndex;
					pIndex = initIndicesLODSingle(pIndex, pLod, pLod + left, pLod + right, pLod + top, pLod + bottom);

					// count - in general count of indices;
					mLodInfoStorage[pLod].mSingleLodInfo[left][right][top][bottom].mCount = pIndex - 
														 mLodInfoStorage[pLod].mSingleLodInfo[left][right][top][bottom].mStart;
				}
			}
		}
	}
	return pIndex;
}

int32_t GeomipGrid::initIndicesLODSingle(uint32_t pIndex, int32_t pLodCore, int32_t pLodLeft, int32_t pLodRight, int32_t pLodTop, int32_t pLodBottom)
{
	// how many steps we need to make in order to seize a quad/tile and step to a next quad;
	int32_t	fanStep = static_cast<int32_t>(pow(2, pLodCore + 1));
	// endPos of current patch. for instance:
	// we compute and going till the last row/column exactly because 2 patches can be overlapped;
	// this way we ensure, that nothing is overlapping 
	int32_t endPos  = mPatchSize - 1 - fanStep;
	
	// loops are going through each quad;
	for (int32_t z = 0; z <= endPos; z += fanStep) 
	{
		for (int32_t x = 0; x <= endPos; x += fanStep)
		{
			// checks if currently we are on the borders or inside of our patch.
			// that was made in order to preclude overlapping 
			/*
				one more note: 
				x == 0 - means we are at the beginning of the patch, so, in forth - 
				we need to check exactly left side, instead of internal side
			*/
			int32_t lLeft   = x == 0	  ? pLodLeft : pLodCore;
			int32_t lRight  = x == endPos ? pLodRight : pLodCore;
			int32_t lBottom = z == 0	  ? pLodBottom : pLodCore;
			int32_t lTop    = z == endPos ? pLodTop : pLodCore;

			pIndex = createTriangleFan(pIndex, pLodCore, pLodLeft, pLodRight, pLodTop, pLodBottom, x, z);
		}
	}

	return pIndex;
}

void GeomipGrid::calcNormals(std::vector<Vertex>& pVertices, std::vector<uint32_t>& pIndices)
{
	uint32_t index = 0;
	for (size_t z = 0; z < mDepth - 1; z += (mPatchSize - 1))
	{
		for (size_t x = 0; x < mWidth - 1; x += (mPatchSize - 1))
		{
			int32_t baseVertex = z * mWidth + x;
			for (size_t i = 0; i < mIndices.size(); i += 3)
			{
				uint32_t index0 = baseVertex + mIndices[i];
				uint32_t index1 = baseVertex + mIndices[i + 1];
				uint32_t index2 = baseVertex + mIndices[i + 2];

				glm::vec3 vector1 = mVertices[index1].mPos - mVertices[index0].mPos;
				glm::vec3 vector2 = mVertices[index2].mPos - mVertices[index0].mPos;
				glm::vec3 normal = glm::cross(vector1, vector2);
				normal = glm::normalize(normal);

				mVertices[index0].mNormals += normal;
				mVertices[index1].mNormals += normal;
				mVertices[index2].mNormals += normal;
			}
		}
	}
	for (auto& i : pVertices)
	{
		i.mNormals = glm::normalize(i.mNormals);
	}
}

void GeomipGrid::initAABB()
{
	glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::min());
	glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::max());
	
	for (auto& vertex : mVertices)
	{
		minAABB.x = std::min(minAABB.x, vertex.mPos.x);
		minAABB.y = std::min(minAABB.y, vertex.mPos.y);
		minAABB.z = std::min(minAABB.z, vertex.mPos.z);

		maxAABB.x = std::max(maxAABB.x, vertex.mPos.x);
		maxAABB.y = std::max(maxAABB.y, vertex.mPos.y);
		maxAABB.z = std::max(maxAABB.z, vertex.mPos.z);
	}

	mAABB = std::make_unique<AABB>(maxAABB, minAABB);
}

uint32_t GeomipGrid::addTriangle(uint32_t pIndex, const std::vector<uint32_t>& pIndices, uint32_t pV1, uint32_t pV2, uint32_t pV3)
{
	assert(pIndex < mIndices.size());
	mIndices[pIndex++] = pV1;
	assert(pIndex < mIndices.size());
	mIndices[pIndex++] = pV2;
	assert(pIndex < mIndices.size());
	mIndices[pIndex++] = pV3;

	return pIndex;
}

uint32_t GeomipGrid::createTriangleFan(uint32_t pIndex, int32_t pLodCore, int32_t pLodLeft, int32_t pLodRight,
									   int32_t pLodTop, int32_t pLodBottom, int32_t pX, int32_t pZ)
{
	/*
		stepLeft, stepRight, etc. = How many vertices to jump based on LOD level;
		indexCenter = The anchor point for all triangles;
		The logic = Walk around the center point creating triangles, adding extra triangles when neighbors have the same LOD (more detail needed);
	*/
	int32_t stepLeft = static_cast<int32_t>(pow(2, pLodLeft));
	int32_t stepRight = static_cast<int32_t>(pow(2, pLodRight));
	int32_t stepTop = static_cast<int32_t>(pow(2, pLodTop));
	int32_t stepBottom = static_cast<int32_t>(pow(2, pLodBottom));
	int32_t stepCenter = static_cast<int32_t>(pow(2, pLodCore));
	
	uint32_t indexCenter = (pZ + stepCenter) * mWidth + pX + stepCenter;

	uint32_t indexTemp1 = pZ * mWidth + pX;
	uint32_t indexTemp2 = (pZ + stepLeft) * mWidth + pX;

	pIndex = addTriangle(pIndex, mIndices, indexCenter, indexTemp1, indexTemp2);

	if (pLodLeft == pLodCore)
	{
		indexTemp1 = indexTemp2;
		indexTemp2 += stepLeft * mWidth;
		
		pIndex = addTriangle(pIndex, mIndices, indexCenter, indexTemp1, indexTemp2);
	}
	
	indexTemp1 = indexTemp2;
	indexTemp2 += stepTop;

	pIndex = addTriangle(pIndex, mIndices, indexCenter, indexTemp1, indexTemp2);

	if (pLodTop == pLodCore)
	{
		indexTemp1 = indexTemp2;
		indexTemp2 += stepTop;

		pIndex = addTriangle(pIndex, mIndices, indexCenter, indexTemp1, indexTemp2);
	}

	indexTemp1 = indexTemp2;
	indexTemp2 -= stepRight * mWidth;

	pIndex = addTriangle(pIndex, mIndices, indexCenter, indexTemp1, indexTemp2);

	if (pLodRight == pLodCore)
	{
		indexTemp1 = indexTemp2;
		indexTemp2 -= stepRight * mWidth;

		pIndex = addTriangle(pIndex, mIndices, indexCenter, indexTemp1, indexTemp2);
	}

	indexTemp1 = indexTemp2;
	indexTemp2 -= stepBottom;

	pIndex = addTriangle(pIndex, mIndices, indexCenter, indexTemp1, indexTemp2);

	if (pLodBottom == pLodCore)
	{
		indexTemp1 = indexTemp2;
		indexTemp2 -= stepBottom;

		pIndex = addTriangle(pIndex, mIndices, indexCenter, indexTemp1, indexTemp2);
	}

	return pIndex;
}

int32_t GeomipGrid::calcNumIndices()
{
	// amount of quads in general
	int32_t numQuads = (mPatchSize - 1) * (mPatchSize - 1);
	// final number of indices (will be returned from the function)
	int32_t numIndices = 0;
	/*
		each patch has 4 neighbors (north, south, west, east). 
		each neighbor has 2 possibilities:
		1. the same lod as current patch;
		2. higher lod, than current patch;
		2(north) * 2(south) * 2(west) * 2(east) = 16;
	*/
	int32_t maxPermutationPerLevel = LEFT_PATCH * RIGHT_PATCH * TOP_PATCH * BOTTOM_PATCH; // 2 * 2 * 2 * 2 = 16;
	const int32_t indicesPerQuad = 6;
	for (int32_t lod = 0; lod <= mMaxLod; ++lod)
	{
		/*
			counting in general amount of indices. for instance:
			256 * 6 * 16 = 24,576 - indices per LOD0;	
			numQuads /= 4 - gives us 64 (4x4 patch);
			and so on
		*/
		numIndices += numQuads * indicesPerQuad * maxPermutationPerLevel;
		numQuads /= 4;
	}
	return numIndices;
}

void GeomipGrid::render(Camera* pCamera, const glm::mat4& pViewProj)
{
	mLodManager.update(pCamera->getPos());
	FrustumCulling frustumCulling(pViewProj);
	mVAO.bind();

	uint32_t numPatches = mNumPatchesX * mNumPatchesZ;
	uint32_t passedNumPatches = 0;

	float patchSize = static_cast<float>(mPatchSize - 1.0f) * mWorldScale;
	float halfPatchSize = patchSize / 2.0f;
	for (uint32_t patchZ = 0; patchZ < mNumPatchesZ; ++patchZ)
	{
		for (uint32_t patchX = 0; patchX < mNumPatchesX; ++patchX)
		{
			int32_t z = patchZ * (mPatchSize - 1);
			int32_t x = patchX * (mPatchSize - 1);

			if (!isPatchInsideFrustum_ViewSpace(x, z, pViewProj))
				continue;

			passedNumPatches++;

			const LodManager::PatchLod& patchLod = mLodManager.getPatchLod(patchX, patchZ);
			int32_t core = patchLod.mCore;
			int32_t left = patchLod.mLeft;
			int32_t right = patchLod.mRight;
			int32_t top = patchLod.mTop;
			int32_t bottom = patchLod.mBottom;

			size_t baseIndex = sizeof(uint32_t) * mLodInfoStorage[core].mSingleLodInfo[left][right][top][bottom].mStart;
			int32_t baseVertex = z * mWidth + x;

			glDrawElementsBaseVertex(GL_TRIANGLES, mLodInfoStorage[core].mSingleLodInfo[left][right][top][bottom].mCount,
				GL_UNSIGNED_INT, (void*)baseIndex, baseVertex);
		}
	}
	std::cout << std::format("PassedNumPatches: {}\tNumPatches: {}\n", passedNumPatches, numPatches);
}

bool GeomipGrid::isPatchInsideFrustum_ViewSpace(int32_t pX, int32_t pZ, const glm::mat4& pViewProj)
{
	int32_t x0 = pX;
	int32_t x1 = pX + mPatchSize - 1;
	int32_t z0 = pZ;
	int32_t z1 = pZ + mPatchSize - 1;

	float heights[3];
	// minimum point
	heights[0] = std::min(mTerrain->getHeight(x0, z0),
						  std::min(mTerrain->getHeight(x0, z1),
								   std::min(mTerrain->getHeight(x1, z0), mTerrain->getHeight(x1, z1))));
	// maximum point
	heights[2] = std::max(mTerrain->getHeight(x0, z0),
						  std::max(mTerrain->getHeight(x0, z1),
								   std::max(mTerrain->getHeight(x1, z0), mTerrain->getHeight(x1, z1))));
	// average point
	heights[1] = (mTerrain->getHeight(x0, z0) + 
				  mTerrain->getHeight(x0, z1) +
				  mTerrain->getHeight(x1, z0) +
				  mTerrain->getHeight(x1, z1)) / 4.0f;

	for (int32_t h = 0; h < std::ssize(heights); ++h) 
	{
		glm::vec3 points[4] = 
		{
			{static_cast<float>(x0) * mWorldScale, heights[h], static_cast<float>(z0) * mWorldScale},
			{static_cast<float>(x0) * mWorldScale, heights[h], static_cast<float>(z1) * mWorldScale},
			{static_cast<float>(x1) * mWorldScale, heights[h], static_cast<float>(z0) * mWorldScale},
			{static_cast<float>(x1) * mWorldScale, heights[h], static_cast<float>(z1) * mWorldScale}
		};

		for (int32_t i = 0; i < 4; ++i)
		{
			if (Utils::getInstance().isPointInsideFrustum(points[i], pViewProj, 8.0f)) 
				return true;
		}
	}
	
	return false;
}

bool GeomipGrid::isPatchInsideFrustum_WorldSpace(int32_t pX, int32_t pZ, const FrustumCulling& pFrustumCulling)
{ 
	int32_t x0 = pX;
	int32_t x1 = pX + mPatchSize - 1;
	int32_t z0 = pZ;
	int32_t z1 = pZ + mPatchSize - 1;

	float h00 = mTerrain->getHeight(x0, z0);
	float h01 = mTerrain->getHeight(x0, z1);
	float h10 = mTerrain->getHeight(x1, z0);
	float h11 = mTerrain->getHeight(x1, z1);
	
	float minHeight = std::min(h00, std::min(h01, std::min(h10, h11)));
	float maxHeight = std::max(h00, std::max(h01, std::max(h10, h11)));
	
	glm::vec3 p00_min = glm::vec3(static_cast<float>(x0) * mWorldScale, minHeight, static_cast<float>(z0) * mWorldScale);
	glm::vec3 p01_min = glm::vec3(static_cast<float>(x0) * mWorldScale, minHeight, static_cast<float>(z1) * mWorldScale);
	glm::vec3 p10_min = glm::vec3(static_cast<float>(x1) * mWorldScale, minHeight, static_cast<float>(z0) * mWorldScale);
	glm::vec3 p11_min = glm::vec3(static_cast<float>(x1) * mWorldScale, minHeight, static_cast<float>(z1) * mWorldScale);

	glm::vec3 p00_max = glm::vec3(static_cast<float>(x0) * mWorldScale, maxHeight, static_cast<float>(z0) * mWorldScale);
	glm::vec3 p01_max = glm::vec3(static_cast<float>(x0) * mWorldScale, maxHeight, static_cast<float>(z1) * mWorldScale);
	glm::vec3 p10_max = glm::vec3(static_cast<float>(x1) * mWorldScale, maxHeight, static_cast<float>(z0) * mWorldScale);
	glm::vec3 p11_max = glm::vec3(static_cast<float>(x1) * mWorldScale, maxHeight, static_cast<float>(z1) * mWorldScale);
	
	bool isInside =
		pFrustumCulling.isPointInsideViewFrustum(p00_min) ||
		pFrustumCulling.isPointInsideViewFrustum(p01_min) ||
		pFrustumCulling.isPointInsideViewFrustum(p10_min) ||
		pFrustumCulling.isPointInsideViewFrustum(p11_min) ||
		pFrustumCulling.isPointInsideViewFrustum(p00_max) ||
		pFrustumCulling.isPointInsideViewFrustum(p01_max) ||
		pFrustumCulling.isPointInsideViewFrustum(p10_max) ||
		pFrustumCulling.isPointInsideViewFrustum(p11_max);

	return false;
}

bool GeomipGrid::isCameraInPatch(const glm::vec3& pCameraPos, int32_t pX, int32_t pZ)
{
	float x0 = static_cast<float>(pX - 2 * mPatchSize) * mWorldScale;
	float x1 = static_cast<float>(pX + 2 * mPatchSize) * mWorldScale;
	float z0 = static_cast<float>(pZ - 2 * mPatchSize) * mWorldScale;
	float z1 = static_cast<float>(pZ + 2 * mPatchSize) * mWorldScale;
	
	bool cameraInPatch = (pCameraPos.x >= x0) &&
						 (pCameraPos.x <= x1) &&
						 (pCameraPos.z >= z0) &&
						 (pCameraPos.z <= z1);

	return cameraInPatch;
}
