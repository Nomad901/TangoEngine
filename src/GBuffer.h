#pragma once
#include <iostream>
#include <format>
#include <array>
#include <vector>

#include "glm.hpp"
#include "glad/glad.h"

#include "Utils.h"

class Shader;

class GBuffer
{
public:
	enum class GBUFFER_TEXTURE_TYPE : uint32_t
	{
		GBUFFER_POSITION = 0,
		GBUFFER_DIFFUSE = 1,
		GBUFFER_NORMAL = 2,
		GBUFFER_NUM_TEXTURES = 3
	};
public:
	GBuffer() = default;
	~GBuffer();

	void init(uint32_t pScreenWidth, uint32_t pScreenHeight);
	void onWindowResize(uint32_t pScreenWidth, uint32_t pScreenHeight);

	void startFrame();
	void bindForGeomPass();
	void bindForStencilPass();
	void bindForLightPass();
	void bindForFinalPass();

	void bind();
	void bind(Shader& pShader, const std::vector<std::string_view>& pTextureNames);
	void bindForWriting();
	void bindForReading(bool pWithTextures);
	void bindForReading(Shader& pShader, const std::vector<std::string_view>& pTextureNames);
	void unbind();
	void unbindForWriting();
	void unbindForReading();
	void setReadBuffer(GBUFFER_TEXTURE_TYPE pTextureType);
	void destroy();

	GLenum getStatusOfFramebuffer() const noexcept;

	void setFreeTextureAttachment(uint32_t pFreeTextureAttachment);
	uint32_t getFreeTextureAttachment() const noexcept; 

	uint32_t getGBuffer() const noexcept;
	uint32_t getGPosBuffer() const noexcept;
	uint32_t getGNormalBuffer() const noexcept;
	uint32_t getGDiffuseBuffer() const noexcept;
	uint32_t getDepthBuffer() const noexcept;

private:
	uint32_t mCounterOfAttachments{};
	uint32_t mGBuffer{};
	uint32_t mDepthBuffer{};
	uint32_t mFinalTexture{};
	std::array<uint32_t, static_cast<uint32_t>(GBUFFER_TEXTURE_TYPE::GBUFFER_NUM_TEXTURES)> mTextures;
};

