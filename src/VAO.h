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
	VAO(const VAO&) = delete;
	VAO& operator=(const VAO&) = delete;
	VAO(VAO&&) = default;
	VAO& operator=(VAO&&) = default;

	void addBuffer(const VBO& pVBO, VBOLayout pVBOLayout);

	void destroy();
	void generate();
	void bind() const;
	void unbind() const;

	uint32_t getID() const noexcept; 

private:
	uint32_t mRendererID;
};

