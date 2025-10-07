#include "Crosshair.h"

Crosshair::Crosshair(float pRadius, const glm::vec3& pColor)
{
	init(pRadius, pColor);
}

void Crosshair::init(float pRadius, const glm::vec3& pColor)
{
	mShader.init(mShader.getResourcePath() + "Shaders/crosshairVert.glsl",
				 mShader.getResourcePath() + "Shaders/crosshair.glsl");

	std::vector<Vertex2D> vertices;
	vertices.reserve(24);
	std::vector<uint32_t> indices;
	indices.reserve(6);

	vertices =
	{
		{glm::vec2(-pRadius,  pRadius),  glm::vec2(0.0f, 1.0f), glm::vec4(pColor.x, pColor.y, pColor.z, 1.0f)},
		{glm::vec2(-pRadius, -pRadius),  glm::vec2(0.0f, 0.0f), glm::vec4(pColor.x, pColor.y, pColor.z, 1.0f)},
		{glm::vec2( pRadius, -pRadius),  glm::vec2(1.0f, 0.0f), glm::vec4(pColor.x, pColor.y, pColor.z, 1.0f)},
		{glm::vec2( pRadius,  pRadius),  glm::vec2(1.0f, 1.0f), glm::vec4(pColor.x, pColor.y, pColor.z, 1.0f)}
	};
	indices =
	{
		0,1,2,
		0,2,3
	};
	mVAO.bind();
	mVBO.init(vertices.data(), vertices.size() * sizeof(Vertex2D), GL_STATIC_DRAW);
	mVBOLayout.pushLayout(GL_FLOAT, 2);
	mVBOLayout.pushLayout(GL_FLOAT, 2);
	mVBOLayout.pushLayout(GL_FLOAT, 4);
	mVAO.addBuffer(mVBO, mVBOLayout);
	mEBO.init(indices.data(), indices.size());
}

void Crosshair::render(uint32_t pWinWidth, uint32_t pWinHeight)
{
	mShader.bind();
	updateUniforms(pWinWidth, pWinHeight);
	mVAO.bind();
	glDrawElements(GL_TRIANGLES, mEBO.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Crosshair::updateUniforms(uint32_t pWinWidth, uint32_t pWinHeight)
{
	mShader.setUniform2fv("uResolution", glm::vec2(pWinWidth, pWinHeight));
}
