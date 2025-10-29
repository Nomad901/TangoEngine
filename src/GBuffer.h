#pragma once
#include <iostream>
#include <format>
#include <array>

#include "glm.hpp"
#include "glad/glad.h"

class GBuffer
{
public:
	enum class GBUFFER_TEXTURE_TYPE : uint32_t
	{
		GBUFFER_POSITION = 0,
		GBUFFER_DIFFUSE = 1,
		GBUFFER_NORMAL = 2,
		GBUFFER_TEXCOORD = 3,
		GBUFFER_NUM_TEXTURES = 4
	};
public:
	GBuffer() = default;
	~GBuffer();

	void init(uint32_t pScreenWidth, uint32_t pScreenHeight);

	void bind();
	void bindForWriting();
	void bindForReading();
	void unbind();
	void unbindForWriting();
	void unbindForReading();
	void setReadBuffer(GBUFFER_TEXTURE_TYPE pTextureType);
	void destroy();

	uint32_t getGBuffer() const noexcept;
	uint32_t getGPosBuffer() const noexcept;
	uint32_t getGNormalBuffer() const noexcept;
	uint32_t getGColorSpecBuffer() const noexcept;
	uint32_t getRBOBuffer() const noexcept;

private:
	uint32_t mGBuffer{};
	uint32_t mDepthBuffer{};
	std::array<uint32_t, static_cast<uint32_t>(GBUFFER_TEXTURE_TYPE::GBUFFER_NUM_TEXTURES)> mTextures;
};

