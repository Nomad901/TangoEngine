#include "Terrain.h"

Terrain::Terrain(float pWorldScale, float pTexScale, std::span<std::filesystem::path> pPaths)
{
	init(pWorldScale, pTexScale, pPaths);
}

Terrain::Terrain(float pWorldScale, float pTexScale)
{
	init(pWorldScale, pTexScale);
}

void Terrain::init(float pWorldScale, float pTexScale, std::span<std::filesystem::path> pPaths)
{
	assert(pPaths.size() <= mTextures.size());
	std::string resourcePath = RESOURCES_PATH;
	mShader.init(resourcePath + "Shaders/terrainVert.glsl", resourcePath + "Shaders/terrainFrag.glsl");
	mHeights =
	{
		130.0f, 
		180.0f, 
	    240.0f, 
		300.0f
	};
	mWorldScale = pWorldScale;
	mTexScale = pTexScale;
	mIsOneTex = false;
	for (size_t i = 0; i < pPaths.size(); ++i)
	{
		mTextures[i] = std::make_unique<Texture2>(pPaths[i], true);
		mTextures[i]->setTarget(GL_TEXTURE_2D);
	}
}

void Terrain::init(float pWorldScale, float pTexScale)
{
    std::string resourcePath = RESOURCES_PATH;
	mShader.init(resourcePath + "Shaders/terrainVert.glsl", resourcePath + "Shaders/terrainFrag.glsl");
	mWorldScale = pWorldScale;
	mTexScale = pTexScale;
	mIsOneTex = true;
}

void Terrain::loadFromFile(const std::filesystem::path& pPath)
{
	loadHeightMapFile(pPath);
	mTriangleList.createTriangleList(mTerrainSize, mTerrainSize, this);
}

void Terrain::setHeights(float pHeight0, float pHeight1, float pHeight2, float pHeight3)
{
	mHeights[0] = pHeight0;
	mHeights[1] = pHeight1;
	mHeights[2] = pHeight2;
	mHeights[3] = pHeight3;
}

void Terrain::setPos(const glm::vec3& pPos)
{
	mPos = pPos;
}

void Terrain::setLight(const glm::vec3& pDirection, float pSoftness)
{
	if (!mSlopeLight)
		mSlopeLight = std::make_unique<SlopeLight>(mHeightMap);
	else
	{
		mSlopeLight.release();
		mSlopeLight = std::make_unique<SlopeLight>(mHeightMap);
	}
	mSlopeLight->init(pDirection, mTerrainSize, pSoftness);
	mTriangleList.initLightFactor(mSlopeLight.get());
	mTriangleList.createGLState();
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
	float interpolatedBottom = leftBottom + (rightBottom - leftBottom) * factorX;
	float interpolatedTop = leftTop + (rightTop - leftTop) * factorX;

	float factorZ = pZ - roundf(pZ);
	float finalHeight = interpolatedBottom + (interpolatedTop - interpolatedBottom) * factorZ;
	
	return finalHeight;
}

void Terrain::render(const glm::mat4& pViewMat, const glm::mat4& pProj, 
					 const glm::vec3& pLightPos)
{
	mShader.bind();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, mPos);
	mShader.setMatrixUniform4fv("uModel", model);
	mShader.setMatrixUniform4fv("uView", pViewMat);
	mShader.setMatrixUniform4fv("uProj", pProj);
	mShader.setUniform1f("uMinHeight", mMinHeight);
	mShader.setUniform1f("uMaxHeight", mMaxHeight);
	mShader.setUniform1i("isSingleTex", mIsOneTex);
	mShader.setUniform3fv("uReversedLightDir", pLightPos);
	if (!mIsOneTex)
	{
		for (size_t i = 0; i < mTextures.size(); ++i)
		{
			mTextures[i]->bind(i);
			mShader.setUniform1i("uTextureHeight" + std::to_string(i), i);
			mShader.setUniform1f("uHeight" + std::to_string(i), mHeights[i]);
		}
	}
	mTriangleList.render();
}

float Terrain::getWorldScale() const noexcept
{
	return mWorldScale;
}

float Terrain::getTextureScale() const noexcept
{
	return mTexScale;
}

int32_t Terrain::getTerrainSize() const noexcept
{
	return mTerrainSize;
}

SlopeLight* Terrain::getSlopeLight() noexcept
{
	return mSlopeLight.get();
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
