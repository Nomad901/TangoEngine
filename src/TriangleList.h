#pragma once
#include <iostream>
#include <format>
#include <vector>

#include "glm/glm.hpp"

#include "VBO.h"
#include "VAO.h"
#include "VBOLayout.h"
#include "EBO.h"

class Terrain;

class TriangleList
{
public:
	TriangleList() = default;
	TriangleList(int32_t pWidth, int32_t pDepth, const Terrain* pTerrain);

	void createTriangleList(int32_t pWidth, int32_t pDepth, const Terrain* pTerrain);
	
	void render();

private:
	void createGLState();
	void initVertices(const Terrain* pTerrain);

private:
	int32_t mWidth{ 0 }, mDepth{ 0 };
	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndices;

	VBO mVBO;
	VBOLayout mVBOLayout;
	VAO mVAO;
	EBO mEBO;
};

