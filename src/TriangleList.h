#pragma once
#include <iostream>
#include <format>
#include <vector>

#include "glm/glm.hpp"

#include "VBO.h"
#include "VAO.h"
#include "VBOLayout.h"
#include "EBO.h"
#include "Light.h"

class Terrain;

class TriangleList
{
public:
	TriangleList() = default;
	TriangleList(int32_t pWidth, int32_t pDepth, Terrain* pTerrain);
	~TriangleList();

	void createTriangleList(int32_t pWidth, int32_t pDepth, Terrain* pTerrain);
	void createGLState();

	void destroy();
	void render();

private:
	void initVertices(const Terrain* pTerrain);
	void initIndices();
	void calcNormals(std::vector<Vertex>& pVertices, std::vector<uint32_t>& pIndices);

private:
	int32_t mWidth{ 0 }, mDepth{ 0 };
	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndices;

	VBO mVBO;
	VBOLayout mVBOLayout;
	VAO mVAO;
	EBO mEBO;
};

