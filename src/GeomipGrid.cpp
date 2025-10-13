#include "GeomipGrid.h"
#include "Terrain.h"

GeomipGrid::GeomipGrid(int32_t pWidth, int32_t pDepth, uint32_t pPatchSize, Terrain* pTerrain)
{
	createGeomipGrid(pWidth, pDepth, pPatchSize, pTerrain);
}

GeomipGrid::~GeomipGrid()
{
	destroy();
}

void GeomipGrid::createGeomipGrid(int32_t pWidth, int32_t pDepth, uint32_t pPatchSize, Terrain* pTerrain)
{
	if ((pWidth - 1) % (pPatchSize - 1) != 0)
	{
		int32_t recommendedWidth = ((pWidth - 1 + pPatchSize - 1) / (pPatchSize - 1)) * (pPatchSize - 1) + 1;
		std::cout << std::format("Width must be divisible by patchSize. Width and patch size, which uve passed: {} / {}\n", pWidth, pPatchSize);
		std::cout << std::format("Recommended width: {}\n", recommendedWidth);
	}
	if ((pDepth - 1) % (pPatchSize - 1) != 0)
	{
		int32_t recommendedDepth = ((pDepth - 1 + pPatchSize - 1) / (pPatchSize - 1)) * (pPatchSize - 1) + 1;
		std::cout << std::format("pDepth must be divisible by patchSize. Depth and patch size, which uve passed: {} / {}\n", pDepth, pPatchSize);
		std::cout << std::format("Recommended pDepth: {}\n", recommendedDepth);
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

	mNumPatchesX = (pWidth - 1) / (pPatchSize - 1);
	mNumPatchesZ = (pDepth - 1) / (pPatchSize - 1);

	float worldScale = pTerrain->getWorldScale();
	mMaxLod = mLodManager.initLodManger(pPatchSize, mNumPatchesX, mNumPatchesZ, worldScale);
	mLodInfoStorage.resize(mMaxLod - 1);
	
	if (!mVertices.empty() || !mIndices.empty())
	{
		mVertices.clear();
		mIndices.clear();
	}
	initVertices(pTerrain);
	initIndices();
	calcNormals(mVertices, mIndices);
	createGLState();
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

void GeomipGrid::render(const glm::vec3& pCameraPos)
{
	mLodManager.update(pCameraPos);
	mVAO.bind();
	
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
	int32_t totalIndicesLod = 0;
	
	for (int32_t left = 0; left < LEFT; ++left)
	{
		for (int32_t right = 0; right < RIGHT; ++right)
		{
			for (int32_t top = 0; top < TOP; ++top)
			{
				for (int32_t bottom = 0; bottom < BOTTOM; ++bottom)
				{
					mLodInfoStorage[pLod].mSingleLodInfo[left][right][top][bottom].mStart = pIndex;
					pIndex = initIndicesLODSingle(pIndex, pLod, pLod + left, pLod + right, pLod + top, pLod + bottom);

					mLodInfoStorage[pLod].mSingleLodInfo[left][right][top][bottom].mCount = pIndex - 
														 mLodInfoStorage[pLod].mSingleLodInfo[left][right][top][bottom].mStart;
					totalIndicesLod += mLodInfoStorage[pLod].mSingleLodInfo[left][right][top][bottom].mCount;
				}
			}
		}
	}

#ifdef DEBUG
	std::cout << std::format("Total amount of indices per lod: {}\n", totalIndicesLod);
#endif // DEBUG

	return pIndex;
}

int32_t GeomipGrid::initIndicesLODSingle(uint32_t pIndex, int32_t pLodCore, int32_t pLodLeft, int32_t pLodRight, int32_t pLodTop, int32_t pLodBottom)
{
	int32_t	fanStep = static_cast<int32_t>(pow(2, pLodCore + 1));
	int32_t endPos  = mPatchSize - 1 - fanStep;
	
	for (int32_t z = 0; z <= endPos; z += fanStep) 
	{
		for (int32_t x = 0; x <= endPos; x += fanStep)
		{
			int32_t lLeft = x == 0 ? pLodLeft : pLodCore;
			int32_t lRight = x == endPos ? pLodRight : pLodCore;
			int32_t lBottom = z == 0 ? pLodBottom : pLodCore;
			int32_t lTop = z == endPos ? pLodTop : pLodCore;

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
	int32_t numQuads = (mPatchSize - 1) * (mPatchSize - 1);
	int32_t numIndices = 0;
	int32_t maxPermutationPerLevel = 16;
	const int32_t indicesPerQuad = 6;
	for (int32_t lod = 0; lod <= mMaxLod; ++lod)
	{
		numIndices += numQuads * indicesPerQuad * maxPermutationPerLevel;
		numQuads /= 4;
	}
	return numIndices;
}
