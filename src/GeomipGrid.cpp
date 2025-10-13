#include "GeomipGrid.h"

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

void GeomipGrid::render()
{
	mVAO.bind();
	for (uint32_t z = 0; z < mDepth - 1; z += (mPatchSize - 1))
	{
		for (uint32_t x = 0; x < mWidth - 1; x += (mPatchSize - 1))
		{
			int32_t baseVertex = z * mWidth + x;
			glDrawElementsBaseVertex(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, NULL, baseVertex);
		}
	}
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

void GeomipGrid::initIndices()
{
	int32_t numQuads = (mPatchSize - 1) * (mPatchSize - 1);
	mIndices.resize(numQuads * 6);
	uint32_t index = 0;
	for (size_t z = 0; z < mPatchSize - 1; z += 2)
	{
		for (size_t x = 0; x < mPatchSize - 1; x += 2)
		{
			uint32_t indexCenter = (z + 1) * mWidth + x + 1;

			//
			// left-bottom vertex;
			//
			uint32_t indexTemp1 = z * mWidth + x;
			//
			// left vertex (from the center vertex);
			//
			uint32_t indexTemp2 = (z + 1) * mWidth + x;
			
			index = addTriangle(index, mIndices, indexCenter, indexTemp1, indexTemp2);
			indexTemp1 = indexTemp2;
			indexTemp2 += mWidth;

			index = addTriangle(index, mIndices, indexCenter, indexTemp1, indexTemp2);
			indexTemp1 = indexTemp2;
			indexTemp2++;

			index = addTriangle(index, mIndices, indexCenter, indexTemp1, indexTemp2);
			indexTemp1 = indexTemp2;
			indexTemp2++;

			index = addTriangle(index, mIndices, indexCenter, indexTemp1, indexTemp2);
			indexTemp1 = indexTemp2;
			indexTemp2 -= mWidth;

			index = addTriangle(index, mIndices, indexCenter, indexTemp1, indexTemp2);
			indexTemp1 = indexTemp2;
			indexTemp2 -= mWidth;

			index = addTriangle(index, mIndices, indexCenter, indexTemp1, indexTemp2);
			indexTemp1 = indexTemp2;
			indexTemp2--;

			index = addTriangle(index, mIndices, indexCenter, indexTemp1, indexTemp2);
			indexTemp1 = indexTemp2;
			indexTemp2--;

			index = addTriangle(index, mIndices, indexCenter, indexTemp1, indexTemp2);
		}
	}
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
