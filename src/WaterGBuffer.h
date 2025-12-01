#pragma once
#include "GBuffer.h"

class WaterGBuffer : public GBuffer
{
public:
	enum class TextureType : uint32_t
	{
		EXTRA_COMPONENT_TEX = 0,
		COLOR_BUFFER_TEX = 1,
		NUM_TEXTURES = 2
	};
public:
	WaterGBuffer() = default;
	WaterGBuffer(uint32_t pScreenWidth, uint32_t pScreenHeight);
	
	void init(uint32_t pScreenWidth, uint32_t pScreenHeight);

	void bind();

	uint32_t getExtraComponentID() const noexcept;
	uint32_t getColorBufferTex() const noexcept;

private:
	uint32_t getIndexTexture(TextureType pTextureType);

	uint32_t generateExtraComponent(uint32_t pScreenWidth, uint32_t pScreenHeight);
	uint32_t generateColorBuffer(uint32_t pScreenWidth, uint32_t pScreenHeight);
		
private:
	std::array<uint32_t, static_cast<uint32_t>(TextureType::NUM_TEXTURES)> mTextures;
	
};

//uniform sampler2D uFinalImage;		- gBuffer
//uniform sampler2D uPosition;			- gBuffer
//uniform sampler2D uNormal;			- gBuffer
//uniform sampler2D uExtraComponent;	- WaterGBuffer
//uniform sampler2D uColorBuffer;		- WaterGBuffer
//
//uniform mat4 uInvView;		- in WaterSSR class, which also contains WaterGBuffer
//uniform mat4 uProjection;		- in WaterSSR class, which also contains WaterGBuffer
//uniform mat4 uInvProjection;	- in WaterSSR class, which also contains WaterGBuffer
//uniform mat4 uView;			- in WaterSSR class, which also contains WaterGBuffer
