#pragma once
#include <iostream>
#include <format>

#include "glad/glad.h"

#include "VBO.h"
#include "VBOLayout.h"

class VAO
{
public:
	VAO();
	~VAO();

	void addBuffer(const VBO& pVBO, VBOLayout pVBOLayout);
	void generate();
	void bind() const;
	void unbind() const;

private:
	uint32_t mRendererID;
};

