#pragma once
#include <iostream>
#include <format>

#include "glm/glm.hpp"
#include "SDL3/SDL.h"
#include "glad/glad.h"

#include "Texture2.h"

class FBO
{
public:
	FBO() = default;
	FBO(uint32_t pWidth, uint32_t pHeight);
	~FBO();

	void init(uint32_t pWidth, uint32_t pHeight);

	void bind();
	void unbind();

	uint32_t getFBO() const noexcept;
	Texture2 getTexture() const noexcept;
	uint32_t getRenderBufferID() const noexcept;
	void getSize(uint32_t& pWidht, uint32_t& pHeight);
	
	void setClearColors(const glm::vec4& pClearColors);
	glm::vec4 getClearColors() const noexcept;

private:
	uint32_t mFBO{};
	uint32_t mRenderBufferID{};
	Texture2 mTexture;

	uint32_t mWidth{}, mHeight{};
	glm::vec4 mClearColors{ 1.0f };
};

