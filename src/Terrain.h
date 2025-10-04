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
			const glm::mat4& pModel, const glm::mat4& pProj,
			const glm::mat4& pView);

	void init(const glm::vec3& pPos, const glm::vec3& pSize, 
			  const std::filesystem::path& pTexturePath,
			  const glm::mat4& pModel, const glm::mat4& pProj,
			  const glm::mat4& pView);
	
	void setPos(const glm::vec3& pPos);
	void setSize(const glm::vec3& pSize);
	void setModel(const glm::mat4& pModel);
	void setProj(const glm::mat4& pProj);
	void setView(const glm::mat4& pView);

	const glm::vec3& getPos() const noexcept;
	const glm::vec3& getSize() const noexcept;
	Texture2& getTexture() noexcept;

	void render();

private:
	void updateUniforms();

private:
	uint32_t mNumStrips, mNumVertsPerStrip;

	glm::vec3 mSize{ 0.0f };
	glm::vec3 mPos{ 0.0f };
	terrainTexture mTexture;	
	Shader mShader;
	VAO mVAO;
	EBO mEBO;
	VBO mVBO;
	VBOLayout mVBOLayout;

	glm::mat4 mModel;
	glm::mat4 mProj;
	glm::mat4 mView;
};

