#pragma once
#include <array>

#include "Texture2.h"
#include "Terrain.h"



class TextureGenerator
{
public:
	TextureGenerator();

	void loadTile(const std::filesystem::path& pPath);

	Texture2* generateTexture(int32_t pTextureSize, Terrain* pTexture, float pMinHeight, float pMaxHeight);
	
private:
	void calculateTextureRegions(float pMinHeight, float pMaxHeight);
	float regionPercent(int32_t pTile, float pHeight);

private:
	const int32_t mMAX_TEXTURE_TILES{ 4 };
		
};

