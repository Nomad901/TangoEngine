#pragma once
#include <iostream>
#include <format>

#include "glm/glm.hpp"
#include "SDL3/SDL.h"
#include "glad/glad.h"

#include "Texture2.h"
#include "Mesh.h"
#include "Shader.h"

class ScreenQuad
{
public:
	ScreenQuad() = default;
	ScreenQuad(uint32_t pScreenWidth, uint32_t pScreenHeight, glm::vec2 pPos, glm::vec2 pSize);

	void init(uint32_t pScreenWidth, uint32_t pScreenHeight, glm::vec2 pPos, glm::vec2 pSize);

	glm::vec2 getSize() const noexcept;
	glm::vec2 getPos() const noexcept;
	
	void render();

private:
	glm::vec2 mPos, mSize;

	VAO mVAO;
	VBO mVBO;
	VBOLayout mVBOLayout;
};

class FBO
{
public:
	FBO() = default;
	FBO(uint32_t pScreenWidth, uint32_t pScreenHeight, glm::vec2 pPos, glm::vec2 pSize);
	~FBO();

	void init(uint32_t pScreenWidth, uint32_t pScreenHeight, glm::vec2 pPos, glm::vec2 pSize);

	void start();
	void stop();
	void stopAndRender();
	void render();

	void bind();
	void unbind();
	void clearColor();

	uint32_t getFBO() const noexcept;
	uint32_t getRenderBufferID() const noexcept;
	glm::vec2 getSize() const noexcept;

	Texture2& getTexture() noexcept;
	ScreenQuad& getScreenQuad() noexcept;
	Shader& getShader() noexcept;
 
	void setClearColors(const glm::vec4& pClearColors);
	glm::vec4 getClearColors() const noexcept;

private: 
	void initShader();

private:
	uint32_t mFBO{};
	uint32_t mRenderBufferID{};
	Texture2 mTexture;
	ScreenQuad mScreenQuad;
	Shader mShader;

	uint32_t mScreenWidth{}, mScreenHeight{};
	glm::vec4 mClearColors{ 1.0f };
};

