//#pragma once
//#include <iostream>
//
//#include <glad/glad.h>
//#include <glm/glm.hpp>
//
//#include "VBO.h"
//#include "VAO.h"
//#include "VBOLayout.h"
//#include "EBO.h"
//#include "Shader.h"
//#include "Mesh.h"
////#include "Texture2.h"
//
//class BatchRenderer
//{
//public:
//	BatchRenderer(uint32_t pMaxQuadQuantity = 500);
//	~BatchRenderer();
//
//	//
//	// initializes VBO, VAO AND SO ON
//	//
//	void init(const Vertex& pInstanceVertex, 
//			  const Shader& pShader, 
//			  const std::vector<Texture2>& pTextures);
//
//	//
//	// Binds everything
//	//
//	void beginRender();
//
//	//
//	// the vertex instance is needed for setting start point;
//	//
//	void setQuad(const Vertex& pVertexInstance, uint32_t pQuantity);
//	void setMeshes(Mesh pMeshInstnace);
//
//	//
//	// unbinds evertything
//	//
//	void endRender();
//	void flush();
//
//	uint32_t getCurrentQuantityQuads() const noexcept;
//
//	void setShader(const Shader& pShader);
//	void setTextures(const std::vector<Texture2>& pTextures);
//	void setMVP(const glm::mat4& pMVP);
//
//private:
//	//
//	// creates quads. 4 quads per cycle;
//	//
//	Vertex* createQuads(Vertex* pVertex, const glm::vec3 pPos, const glm::vec4 pColor, float pTexInd);
//
//private:
//	VAO mVAO;
//	VBO mVBO;
//	VBOLayout mVBOLayout;
//	EBO mEBO;
//	Shader mShader;
//
//	std::vector<Vertex> mVertices;
//	std::vector<uint32_t> mIndices;
//	std::vector<Texture2> mTextures;
//
//	glm::mat4 mMVP;
//
//	uint32_t mMaxQuadQuantity{}, mMaxVerticesQuant{}, mMaxIndicesQuantity{};
//	uint32_t mCurrenQuantityQuads{};
//};
//
