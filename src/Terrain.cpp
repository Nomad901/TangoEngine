#include "Terrain.h"

Terrain::Terrain()
{
	std::string resourcePath = RESOURCES_PATH;
	mShader.init(resourcePath + "Shaders/terrainVert.glsl", resourcePath + "Shaders/terrainFrag.glsl");
}

void Terrain::loadFromFile(const std::filesystem::path& pPath)
{
	loadHeightMapFile(pPath);
	mTriangleList.createTriangleList(mTerrainSize, mTerrainSize, this);
}

float Terrain::getHeight(int32_t pX, int32_t pZ) const
{
	return mHeightMap[pX][pZ];
}

void Terrain::render(const glm::mat4& pViewMat)
{
	mShader.bind();
	mShader.setMatrixUniform4fv("uView", pViewMat);
	mTriangleList.render();
}

void Terrain::loadHeightMapFile(const std::filesystem::path& pPath)
{
	int64_t fileSize = 0;
	std::vector<float> heightData = Utils::getInstance().readFromBinaryFile2Float(pPath, fileSize);

	size_t numFloats = heightData.size();
	mTerrainSize = static_cast<int32_t>(glm::sqrt(numFloats));

	mHeightMap.resize(mTerrainSize);
	for (size_t i = 0; i < mTerrainSize; ++i)
	{
		mHeightMap[i].resize(mTerrainSize);
		for (size_t j = 0; j < mTerrainSize; ++j)
		{
			mHeightMap[i][j] = heightData[i * mTerrainSize + j];
		}
	}
}
