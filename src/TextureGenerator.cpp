#include "TextureGenerator.h"

void TextureGenerator::loadTile(const std::filesystem::path& pPath)
{
	if (mNumTextureTiles >= mMAX_TEXTURE_TILES)
	{
		std::cout << std::format("Current number of texture tiles is more, than could be! Last file: {}\n", pPath.string());
		return;
	}
	mTextureTiles[mNumTextureTiles].mSTBImage.load(pPath);
	mNumTextureTiles++;
}

void TextureGenerator::loadTiles(const std::vector<std::filesystem::path>& pPaths)
{
	for (auto& path : pPaths)
	{
		if (mNumTextureTiles >= mMAX_TEXTURE_TILES)
		{
			std::cout << std::format("Current number of texture tiles is more, than could be! Last file: {}\n", path.string());
			break;
		}
		mTextureTiles[mNumTextureTiles].mSTBImage.load(path);
		mNumTextureTiles++;
	}
}

Texture2* TextureGenerator::generateTexture(int32_t pTextureSize, Terrain* pTerrain, float pMinHeight, float pMaxHeight)
{
	assert(mNumTextureTiles > 0);

	calculateTextureRegions(pMinHeight, pMaxHeight);

	int32_t BPP = 3;
	int32_t textureBytes = pTextureSize * pTextureSize * BPP;
	uint8_t* textureData = (uint8_t*)malloc(textureBytes);
	uint8_t* tmpObject = textureData;

	float heightMapToTextureRatio = static_cast<float>(pTerrain->getTerrainSize()) / static_cast<float>(pTextureSize);
	
	for (int32_t y = 0; y < pTextureSize; ++y)
	{
		for (int32_t x = 0; x < pTextureSize; ++x)
		{
			float interpolatedHeight = pTerrain->getHeightInterpolated(static_cast<float>(x) * heightMapToTextureRatio,
																	   static_cast<float>(y) * heightMapToTextureRatio);
			float red   = 0.0f;
			float green = 0.0f;
			float blue  = 0.0f;
			
			for (int32_t tile = 0; tile < mNumTextureTiles; ++tile)
			{
				glm::vec3 color = mTextureTiles[tile].mSTBImage.getColor(x, y);
				float blendFactor = regionPercent(tile, interpolatedHeight);
				red   += blendFactor * color.r;
				green += blendFactor * color.g;
				blue  += blendFactor * color.b;
			}
			assert(red > 255.0f || green > 255.0f || blue > 255.0f);

			tmpObject[0] = static_cast<uint8_t>(red);
			tmpObject[1] = static_cast<uint8_t>(green);
			tmpObject[2] = static_cast<uint8_t>(blue);

			tmpObject += 3;
		}
	}
	
	std::unique_ptr<Texture2> texture = std::make_unique<Texture2>(GL_TEXTURE_2D);
	
	std::string resourcesPath = RESOURCES_PATH;
	stbi_write_png(std::string(resourcesPath + "texture.png").c_str(), pTextureSize, pTextureSize, BPP, textureData, pTextureSize * BPP);

	bool isRGB = false;
	texture->loadRaw(pTextureSize, pTextureSize, BPP, textureData, isRGB);

	free(textureData);

	return texture.release();
}

void TextureGenerator::calculateTextureRegions(float pMinHeight, float pMaxHeight)
{
	float heightRange = pMaxHeight - pMinHeight;

	float rangePerTile = heightRange / mNumTextureTiles;
	float reminder = heightRange - rangePerTile * mNumTextureTiles;
	if (reminder < 0.0f)
	{
		std::cout << std::format("Reminder is negative! Can not continue! File [{}] Line [{}]\n", __FILE__, __LINE__);
		return;
	}
	
	float lastHeight = -1.0f;
	for (int32_t i = 0; i < mNumTextureTiles; ++i)
	{
		mTextureTiles[i].mTerrainHeightDesc.mLow = lastHeight + 1.0f;
		lastHeight += rangePerTile;
		mTextureTiles[i].mTerrainHeightDesc.mOptimal = lastHeight;
		mTextureTiles[i].mTerrainHeightDesc.mHigh = mTextureTiles[i].mTerrainHeightDesc.mOptimal + rangePerTile;
	}
}

float TextureGenerator::regionPercent(int32_t pTile, float pHeight)
{
	float heightPercent = 0.0f;

	if (heightPercent < mTextureTiles[pTile].mTerrainHeightDesc.mLow)
		heightPercent = 0.0f;
	else if (heightPercent > mTextureTiles[pTile].mTerrainHeightDesc.mHigh)
		heightPercent = 0.0f;
	else if (heightPercent < mTextureTiles[pTile].mTerrainHeightDesc.mOptimal)
	{
		float numerator = pHeight - mTextureTiles[pTile].mTerrainHeightDesc.mLow;
		float denominator = mTextureTiles[pTile].mTerrainHeightDesc.mOptimal - mTextureTiles[pTile].mTerrainHeightDesc.mLow;
		heightPercent = numerator / denominator;
	}
	else if (heightPercent >= mTextureTiles[pTile].mTerrainHeightDesc.mOptimal)
	{
		float numerator = mTextureTiles[pTile].mTerrainHeightDesc.mHigh - pHeight;
		float denominator = mTextureTiles[pTile].mTerrainHeightDesc.mHigh - mTextureTiles[pTile].mTerrainHeightDesc.mOptimal;
		heightPercent = numerator / denominator;
	}
	else
		std::cout << std::format("Something is wrong with region percentage! File [{}] Line [{}] Tile [{}] Height [{}]\n", __FILE__, __LINE__,
																														   pTile, pHeight);

	return heightPercent;
}
