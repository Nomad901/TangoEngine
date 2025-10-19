#include "TriangleList.h"
#include "Terrain.h"

TriangleList::TriangleList(int32_t pWidth, int32_t pDepth, Terrain* pTerrain)
{
	createTriangleList(pWidth, pDepth, pTerrain);
}

TriangleList::~TriangleList()
{
	destroy();
}

void TriangleList::createTriangleList(int32_t pWidth, int32_t pDepth, Terrain* pTerrain)
{
	mWidth = pWidth;
	mDepth = pDepth;
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

void TriangleList::destroy()
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

void TriangleList::render()
{
	mVAO.bind();
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
}

void TriangleList::createGLState()
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

void TriangleList::initVertices(const Terrain* pTerrain)
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

void TriangleList::initIndices()
{
	int32_t numQuads = (mWidth - 1) * (mDepth - 1);
	mIndices.resize(numQuads * 6);
	uint32_t index = 0;
	for (int32_t z = 0; z < mDepth - 1; ++z)
	{
		for (int32_t x = 0; x < mWidth - 1; ++x)
		{
			uint32_t indexBottomLeft = z * mWidth + x;
			uint32_t indexTopLeft = (z + 1) * mWidth + x;
			uint32_t indexTopRight = (z + 1) * mWidth + x + 1;
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

void TriangleList::calcNormals(std::vector<Vertex>& pVertices, std::vector<uint32_t>& pIndices)
{
	for (uint32_t i = 0; i < pIndices.size(); i += 3)
	{
		uint32_t index0 = pIndices[i];
		uint32_t index1 = pIndices[i + 1];
		uint32_t index2 = pIndices[i + 2];
		glm::vec3 firstVector = pVertices[index1].mPos - pVertices[index0].mPos;
		glm::vec3 secondVector = pVertices[index2].mPos - pVertices[index0].mPos;
		glm::vec3 normal = glm::cross(firstVector, secondVector);
		normal = glm::normalize(normal);

		pVertices[index0].mNormals += normal;
		pVertices[index1].mNormals += normal;
		pVertices[index2].mNormals += normal;
	}

	for (auto& i : pVertices)
	{
		i.mNormals = glm::normalize(i.mNormals);
	}
}
