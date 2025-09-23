#pragma once
#include <iostream>
#include <memory>

#include "glm.hpp"
#include "VBO.h"
#include "VAO.h"
#include "VBOLayout.h"
#include "EBO.h"
#include "Shader.h"
#include "Texture2.h"
#include "Primitive.h"
#include "Material.h"

class Mesh
{
public:
	Mesh() = default;
	Mesh(const std::vector<Vertex>& pVertices,
		 const std::vector<uint32_t>& pIndices,
		 const std::vector<Texture2>& pTexture);
	Mesh(const std::vector<Vertex>& pVertices,
		 const std::vector<uint32_t>& pIndices);
	Mesh(const std::weak_ptr<Primitive>& pPrimitive,
		 const std::vector<Texture2>& pTexture);
	Mesh(const std::weak_ptr<Primitive>& pPrimitive);

	void init(const std::vector<Vertex>& pVertices,
			  const std::vector<uint32_t>& pIndices, 
			  const std::vector<Texture2>& pTexture);
	void init(const std::vector<Vertex>& pVertices,
			  const std::vector<uint32_t>& pIndices);
	void init(const std::weak_ptr<Primitive>& pPrimitive,
			  const std::vector<Texture2>& pTextures);
	void init(const std::weak_ptr<Primitive>& pPrimitive);

	void initMVP(const glm::mat4 pProjMatrix, const glm::mat4& pViewMatrix,
				 const glm::vec3& pTranslation, const std::pair<float, glm::vec3>& pDegreeRotate,
				 const glm::vec3& pScale);
	void setModelMatrix(const glm::mat4& pModelMatrix);
	void setViewMatrix(const glm::mat4& pViewMatrix);
	void setProjMatrix(const glm::mat4& pProjMatrix);
	void setMVP(const glm::mat4& pModelMatrix,
				const glm::mat4& pViewMatrix,
				const glm::mat4& pProjMatrix);
	const glm::mat4& getModelMatrix() const noexcept;
	const glm::mat4& getViewMatrix() const noexcept;
	const glm::mat4& getProjMatrix() const noexcept;
	const glm::mat4& getMVP();

	void recomputeMVP();

	void setVertices(const std::vector<Vertex>& pVertices);
	void setIndices(const std::vector<uint32_t>& pIndices);
	void setTextures(const std::vector<Texture2>& pTextures);
	std::vector<Vertex>& getVertices();
	std::vector<uint32_t>& getIndices();
	std::vector<Texture2>& getTextures();
	Primitive& getPrimitive();

	void setPos(const glm::vec3& pPos);
	void setSize(const glm::vec3& pSize);
	const glm::vec3& getPos() const noexcept;
	const glm::vec3& getSize() const noexcept;

	const VAO& getVAO() const noexcept;
	const VBO& getVBO() const noexcept;
	const EBO& getEBO() const noexcept;
	const VBOLayout& getVBOLayout() const noexcept;

	// 
	// sets uniforms for the usual shader
	//
	void setUniforms(Shader& pShader, const glm::vec3& pColor);
	//
	// sets uniforms for normals-shader;
	//
	void setUniformsNormals(Shader& pShader, const glm::vec3& pColor);

	void takeMesh(bool pTake);
	bool meshIsTaken() const noexcept;

	void draw();
	void drawInstances(uint32_t pNumber);
	void drawInFrameBuffer(Texture2& pTexture);
	void drawSkybox();

private:
	void rebuildMatrix(); 

private:
	bool mIsTaken{ false };

	std::shared_ptr<Primitive> mPrimitive;
	std::vector<Texture2> mTextures;
	Texture2 mTexture;

	VAO mVAO;
	VBO mVBO;
	EBO mEBO;
	VBOLayout mVBOLayout;

	glm::mat4 mModelMatrix{ 1.0f }, mViewMatrix{ 1.0f }, mProjMatrix{ 1.0f };
	glm::mat4 mMVP{ 1.0f };
	
	std::pair<float, glm::vec3> mRotate;
	glm::vec3 mPos{ 1.0f };
	glm::vec3 mSize{ 1.0f };
};

