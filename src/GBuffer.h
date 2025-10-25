#pragma once
#include <iostream>
#include <format>
#include <array>

#include "glm.hpp"
#include "glad/glad.h"

class GBuffer
{
public:
	GBuffer() = default;
	~GBuffer();

	void init(uint32_t pScreenWidth, uint32_t pScreenHeight);

	void bind();
	void unbind();
	void destroy();

	uint32_t getGBuffer() const noexcept;
	uint32_t getGPosBuffer() const noexcept;
	uint32_t getGNormalBuffer() const noexcept;
	uint32_t getGColorSpecBuffer() const noexcept;
	uint32_t getRBOBuffer() const noexcept;
	
private:
	uint32_t mGBuffer{};
	uint32_t mGPos{}, mGNormal{}, mGColorSpec{};
	uint32_t mRBO{};
};

