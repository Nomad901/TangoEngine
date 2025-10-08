#include "TriangleList.h"
#include "Terrain.h"

TriangleList::TriangleList(int32_t pWidth, int32_t pDepth, const Terrain* pTerrain)
{
	createTriangleList(pWidth, pDepth, pTerrain);
}

void TriangleList::createTriangleList(int32_t pWidth, int32_t pDepth, const Terrain* pTerrain)
{
	mWidth = pWidth;
	mDepth = pDepth;
	initVertices(pTerrain);
	createGLState();
}

void TriangleList::render()
{
	mVAO.bind();
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
}

void TriangleList::createGLState()
{
	mVAO.bind();
	mVBO.init(mVertices, GL_STATIC_DRAW);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 4);
	mVBOLayout.pushLayout(GL_FLOAT, 2);
	mVAO.addBuffer(mVBO, mVBOLayout);
	mEBO.init(mIndices.data(), mIndices.size());
}

void TriangleList::initVertices(const Terrain* pTerrain)
{
	//
	// vertices
	//
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

	//
	// indices
	//
	int32_t numQuads = (mWidth - 1) * (mDepth - 1);
	mIndices.resize(numQuads * 6);
	index = 0;
	for (int32_t z = 0; z < mDepth - 1; ++z)
	{
		for (int32_t x = 0; x < mWidth - 1; ++x)
		{
			uint32_t indexBottomLeft  = z * mWidth + x;
			uint32_t indexTopLeft     = (z + 1) * mWidth + x;
			uint32_t indexTopRight    = (z + 1) * mWidth + x + 1;
			uint32_t indexBottomRight = z * mWidth + x + 1;

			mIndices[index++] = indexBottomLeft;
			mIndices[index++] = indexTopLeft;
			mIndices[index++] = indexTopRight;

			mIndices[index++] = indexBottomLeft;
			mIndices[index++] = indexTopRight;
			mIndices[index++] = indexBottomRight;
		}
	}
}