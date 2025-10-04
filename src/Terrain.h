#pragma once
#include <iostream>
#include <vector>
#include "string"

#include "glm/glm.hpp"

#include "Mesh.h"
#include "Shader.h"

class Terrain
{
public:
	Terrain() = default;
	Terrain(const glm::vec3& pPos, const glm::vec3& pSize, 
			const std::filesystem::path& pTexturePath,
			const glm::mat4& pProj);

	void init(const glm::vec3& pPos, const glm::vec3& pSize, 
			  const std::filesystem::path& pTexturePath,
			  const glm::mat4& pProj);
	
	void setPos(const glm::vec3& pPos);
	void setSize(const glm::vec3& pSize);
	void setProj(const glm::mat4& pProj);

	const glm::vec3& getPos() const noexcept;
	const glm::vec3& getSize() const noexcept;
	Texture2& getTexture() noexcept;

	void render(const glm::mat4& pViewMatrix);

private:
	void updateUniforms();

private:
	uint32_t mNumStrips, mNumVertsPerStrip;

	std::vector<Vertex> mVertices;
	uint32_t mRez;

	glm::vec3 mSize{ 0.0f };
	glm::vec3 mPos{ 0.0f };
	Texture2 mTexture;	
	Shader mShader;
	VAO mVAO;
	EBO mEBO;
	VBO mVBO;
	VBOLayout mVBOLayout;

	glm::mat4 mProj;
	glm::mat4 mView;
};

