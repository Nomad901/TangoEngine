#pragma once
#include <iostream>
#include <format>
#include <array>

#include "Texture2.h"
#include "Terrain.h"
#include "STBImage.h"

struct TerrainHeightDesc
{
	float mLow = 0.0f;
	float mOptimal = 0.0f;
	float mHigh = 0.0f;

	void print()
	{
		std::cout << std::format("Low: {}\tOptimal: {}\tHigh: {}\n", mLow, mOptimal, mHigh);
	}
};

struct TextureTile
{
	STBImage mSTBImage;
	TerrainHeightDesc mTerrainHeightDesc;
};

class TextureGenerator
{
public:
	TextureGenerator() = default;

	void loadTile(const std::filesystem::path& pPath);
	void loadTiles(const std::vector<std::filesystem::path>& pPaths);

	Texture2* generateTexture(int32_t pTextureSize, Terrain* pTerrain, float pMinHeight, float pMaxHeight);
	
private:
	void calculateTextureRegions(float pMinHeight, float pMaxHeight);
	float regionPercent(int32_t pTile, float pHeight);

private:
	static const int32_t mMAX_TEXTURE_TILES{ 4 };
	std::array<TextureTile, mMAX_TEXTURE_TILES> mTextureTiles;
	int32_t mNumTextureTiles{ 0 };
};

