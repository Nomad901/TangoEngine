#pragma once

#include "Mesh.h"

struct Vertex2D
{
	glm::vec2 mPos;
	glm::vec2 mTexture;
	glm::vec4 mColor;
};

class Crosshair
{
public:
	Crosshair() = default;
	Crosshair(float pRadius, const glm::vec3& pColor);

	void init(float pRadius, const glm::vec3& pColor);

	void render(uint32_t pWinWidth, uint32_t pWinHeight);

private:
	void updateUniforms(uint32_t pWinWidth, uint32_t pWinHeight);

private:
	float mRadius;
	glm::vec3 mColor;

	Shader mShader;

	VAO mVAO;
	VBO mVBO;
	VBOLayout mVBOLayout;
	EBO mEBO;
};

