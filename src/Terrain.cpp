#include "Terrain.h"

Terrain::Terrain(float pWorldScale)
{
	init(pWorldScale);
}

void Terrain::init(float pWorldScale)
{
	std::string resourcePath = RESOURCES_PATH;
	mShader.init(resourcePath + "Shaders/terrainVert.glsl", resourcePath + "Shaders/terrainFrag.glsl");
	mWorldScale = pWorldScale;
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

float Terrain::getHeightInterpolated(float pX, float pZ) const
{
	float leftBottom = getHeight(static_cast<int32_t>(pX), static_cast<int32_t>(pZ));
	if (static_cast<int32_t>(pX) >= mTerrainSize ||
		static_cast<int32_t>(pZ) >= mTerrainSize)
	{
		return leftBottom;
	}
	float rightBottom = getHeight(static_cast<int32_t>(pX) + 1, static_cast<int32_t>(pZ));
	float leftTop = getHeight(static_cast<int32_t>(pX), static_cast<int32_t>(pZ) + 1);
	float rightTop = getHeight(static_cast<int32_t>(pX) + 1, static_cast<int32_t>(pZ) + 1);
	
	float factorX = pX - roundf(pX);
	float interpolatedBottom = (rightBottom - leftBottom) * factorX - leftBottom;
	float interpolatedTop = (rightTop - leftTop) * factorX - leftTop;

	float factorZ = pZ - roundf(pZ);
	float finalHeight = (interpolatedTop - interpolatedBottom) * factorZ - interpolatedBottom;
	
	return finalHeight;
}

void Terrain::render(const glm::mat4& pViewMat, const glm::mat4& pProj)
{
	mShader.bind();
	mShader.setMatrixUniform4fv("uView", pViewMat);
	mShader.setMatrixUniform4fv("uProj", pProj);
	mShader.setUniform1f("uMinHeight", mMinHeight);
	mShader.setUniform1f("uMaxHeight", mMaxHeight);
	mTriangleList.render();
}

float Terrain::getWorldScale() const noexcept
{
	return mWorldScale;
}

int32_t Terrain::getTerrainSize() const noexcept
{
	return mTerrainSize;
}

void Terrain::loadHeightMapFile(const std::filesystem::path& pPath)
{
	int64_t fileSize = 0;
	std::vector<float> heightData;
	if (pPath.extension() == ".save")
		heightData = Utils::getInstance().readFromBinaryFile2Float(pPath, fileSize);
	else
		heightData = Utils::getInstance().readFromPNGFile2Float(pPath, fileSize);

	size_t numFloats = heightData.size();
	mTerrainSize = static_cast<int32_t>(glm::sqrt(numFloats));

	// initialization of height map;
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

void Terrain::getMinMax(float& pMin, float& pMax)
{
	pMax = pMin = mHeightMap[0][0];
	for (size_t i = 1; i < mHeightMap.size(); ++i)
	{
		for (size_t j = 0; j < mHeightMap[i].size(); ++j)
		{
			if (mHeightMap[i][j] < pMin)
				pMin = mHeightMap[i][j];
			if (mHeightMap[i][j] > pMax)
				pMax = mHeightMap[i][j];
		}
	}
}

void Terrain::normalize(float pMinRange, float pMaxRange)
{
	float min{ 0.0f }, max{ 0.0f };
	getMinMax(min, max);

	if (max < min)
		return;

	float minMaxDelta = max - min;
	float minMaxRange = pMaxRange - pMinRange;

	for (int32_t i = 0; i < mHeightMap.size(); ++i)
	{
		for (size_t j = 0; j < mHeightMap[i].size(); ++j)
		{
			mHeightMap[i][j] = ((mHeightMap[i][j] - min)/minMaxDelta) * minMaxRange + pMinRange;
		}
	}
}
