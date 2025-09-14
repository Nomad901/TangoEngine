#include "EBO.h"

EBO::EBO(const void* pData, uint32_t pCount)
{
	init(pData, pCount);
}

EBO::~EBO()
{
	glDeleteBuffers(1, &mRendererID);
}

void EBO::init(const void* pData, uint32_t pCount)
{
	mCount = pCount;

	glGenBuffers(1, &mRendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pCount * sizeof(uint32_t), pData, GL_STATIC_DRAW);
	//glNamedBufferData(GL_ELEMENT_ARRAY_BUFFER, pCount * sizeof(uint32_t), pData, GL_STATIC_DRAW);
}

void EBO::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
}

void EBO::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t EBO::getCount() const noexcept
{
	return mCount;
}
