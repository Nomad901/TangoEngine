#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &mRendererID);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &mRendererID);
}

void VAO::addBuffer(const VBO& pVBO, VBOLayout pVBOLayout)
{
	bind();
	pVBO.bind();
	auto& elements = pVBOLayout.getComponentsOfLayout();
	uint32_t stride = 0;
	for (size_t i = 0; i < elements.size(); ++i)
	{
		auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.mCount, element.mType, element.mNormalized, pVBOLayout.getCount(), (void*)stride);
		stride += element.mCount * VBOLayoutComponents::sizeOf(element.mType);
	}
}

void VAO::destroy()
{
	glDeleteVertexArrays(1, &mRendererID);
}

void VAO::generate()
{
	if (mRendererID)
		destroy();
	glGenVertexArrays(1, &mRendererID);
}

void VAO::bind() const
{
	glBindVertexArray(mRendererID);
}

void VAO::unbind() const
{
	glBindVertexArray(0);
}

uint32_t VAO::getID() const noexcept
{
	return mRendererID;
}
