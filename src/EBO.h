#pragma once
#include <iostream>

#include "glad/glad.h"

class EBO
{
public:
	EBO() = default;
	EBO(const void* pData, uint32_t pCount);
	~EBO();
	EBO(const EBO&) = delete;
	EBO& operator=(const EBO&) = delete;
	EBO(EBO&&) = default;
	EBO& operator=(EBO&&) = default;

	void init(const void* pData, uint32_t pCount);

	void bind();
	void unbind();

	uint32_t getCount() const noexcept;

private:
	uint32_t mRendererID;
	uint32_t mCount{};

};

