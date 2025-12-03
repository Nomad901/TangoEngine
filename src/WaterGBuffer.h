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
	enum class BindingType 
	{
		USUAL = 0,
		READ  = 1,
		WRITE = 2
	};
public:
	WaterGBuffer() = default;
	WaterGBuffer(uint32_t pScreenWidth, uint32_t pScreenHeight);
	~WaterGBuffer();

	void init(uint32_t pScreenWidth, uint32_t pScreenHeight);

	void bind(BindingType pBindingType);
	void unbind();
	void bindTextures();
	void unbindTextures();

	uint32_t getExtraComponentBuffer() const noexcept;
	uint32_t getColorBufferBuffer() const noexcept;

private:
	uint32_t getIndexTexture(TextureType pTextureType) const noexcept;

	void generateExtraComponent(uint32_t pScreenWidth, uint32_t pScreenHeight);
	void generateColorBuffer(uint32_t pScreenWidth, uint32_t pScreenHeight);
		
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
