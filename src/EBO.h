#pragma once
#include <iostream>

#include "glad/glad.h"

class EBO
{
public:
	EBO() = default;
	EBO(const void* pData, uint32_t pCount);
	~EBO();

	void init(const void* pData, uint32_t pCount);

	void bind();
	void unbind();

	uint32_t getCount() const noexcept;

private:
	uint32_t mRendererID;
	uint32_t mCount{};

};

