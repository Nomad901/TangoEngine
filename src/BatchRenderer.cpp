//#include "BatchRenderer.h"
//
//BatchRenderer::BatchRenderer(uint32_t pMaxQuadQuantity)
//{
//	mMaxQuadQuantity = pMaxQuadQuantity;
//	mMaxVerticesQuant = mMaxQuadQuantity * 4;
//	mMaxIndicesQuantity = mMaxQuadQuantity * 6;
//}
//
//BatchRenderer::~BatchRenderer()
//{
//
//}
//
//void BatchRenderer::init(const Vertex& pInstanceVertex,
//						 const Shader& pShader,
//						 const std::vector<Texture2>& pTextures)
//{
//	mShader = pShader;
//	mTextures = pTextures;
//	mVAO.bind();
//	mIndices.resize(mMaxIndicesQuantity);
//	uint32_t offset = 0;
//	for (uint32_t i = 0; i < mIndices.size(); i += 6)
//	{
//		mIndices[0 + i] = 0 + offset;
//		mIndices[1 + i] = 1 + offset;
//		mIndices[2 + i] = 2 + offset;
//		mIndices[3 + i] = 2 + offset;
//		mIndices[4 + i] = 3 + offset;
//		mIndices[5 + i] = 0 + offset;
//		offset += 4;
//	}
//	
//	auto Vertex = mVertices.data();
//	for (uint32_t y = 0; y < mMaxQuadQuantity; ++y)
//	{
//		for (uint32_t i = 0; i < mMaxQuadQuantity; ++i)
//		{
//			Vertex = createQuads(Vertex, pInstanceVertex.mPos, pInstanceVertex.mColor, (y + i) % 2);
//		}
//	}
//
//	mVBO.init(mVertices, GL_DYNAMIC_DRAW);
//	mEBO.init(mIndices.data(), mIndices.size());
//	mVBOLayout.pushLayout(GL_FLOAT, 3);
//	mVBOLayout.pushLayout(GL_FLOAT, 4);
//	mVBOLayout.pushLayout(GL_FLOAT, 2);
//	mVBOLayout.pushLayout(GL_FLOAT, 1);
//	mVAO.addBuffer(mVBO, mVBOLayout);
//}
//
//void BatchRenderer::beginRender()
//{
//	mVAO.bind();
//	mVBO.bind();
//	mEBO.bind();
//	mShader.bind();
//	for (size_t i = 0; i < mTextures.size(); ++i)
//	{
//		//mTextures[i].bind(i);
//	}
//}
//
//void BatchRenderer::setQuad(const Vertex& pVertexInstance, uint32_t pQuantity)
//{
//	mIndices.resize(pQuantity * 6);
//	uint32_t offset = 0;
//	for (uint32_t i = 0; i < mIndices.size(); i += 6)
//	{
//		mIndices[0 + i] = 0 + offset;
//		mIndices[1 + i] = 1 + offset;
//		mIndices[2 + i] = 2 + offset;
//		mIndices[3 + i] = 2 + offset;
//		mIndices[4 + i] = 3 + offset;
//		mIndices[5 + i] = 0 + offset;
//		offset += 4;
//	}
//
//	mVertices.resize(pQuantity);
//	auto Vertex = mVertices.data();
//	for (uint32_t y = 0; y < pQuantity; ++y)
//	{
//		for (uint32_t i = 0; i < pQuantity; ++i)
//		{
//			Vertex = createQuads(Vertex, pVertexInstance.mPos, pVertexInstance.mColor, (y + i) % 2);
//		}
//	}
//}
//
//void BatchRenderer::setMeshes(Mesh pMeshes)
//{
//	mIndices = pMeshes.getIndices();
//	mVertices = pMeshes.getVertices();
//}
//
//void BatchRenderer::endRender()
//{
//	mVAO.unbind();
//	mVBO.unbind();
//	mEBO.unbind();
//	mShader.unbind();
//	for (size_t i = 0; i < mTextures.size(); ++i)
//	{
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		//mTextures[i].unbind();
//	}
//}
//
//void BatchRenderer::flush()
//{
//}
//
//uint32_t BatchRenderer::getCurrentQuantityQuads() const noexcept
//{
//	return mCurrenQuantityQuads;
//}
//
//void BatchRenderer::setShader(const Shader& pShader)
//{
//	mShader = pShader;
//}
//
//void BatchRenderer::setTextures(const std::vector<Texture2>& pTextures)
//{
//	mTextures = pTextures;
//}
//
//void BatchRenderer::setMVP(const glm::mat4& pMVP)
//{
//	mMVP = pMVP;
//}
//
//Vertex* BatchRenderer::createQuads(Vertex* pVertex, const glm::vec3 pPos, const glm::vec4 pColor, float pTexInd)
//{
//	size_t size = 1.0f;
//	pVertex->mPos = glm::vec3(pPos.x, pPos.y, pPos.z);
//	pVertex->mColor = glm::vec4(pColor.x, pColor.y, pColor.z, pColor.w);
//	pVertex->mTexCoord = glm::vec2(0.0f, 0.0f);
//	//pVertex->mTexID = pTexInd;
//	pVertex++;
//
//	pVertex->mPos = glm::vec3(pPos.x + size, pPos.y, pPos.z);
//	pVertex->mColor = glm::vec4(pColor.x, pColor.y, pColor.z, pColor.w);
//	pVertex->mTexCoord = glm::vec2(1.0f, 0.0f);
//	//pVertex->mTexID = pTexInd;
//	pVertex++;
//
//	pVertex->mPos = glm::vec3(pPos.x + size, pPos.y + size, pPos.z);
//	pVertex->mColor = glm::vec4(pColor.x, pColor.y, pColor.z, pColor.w);
//	pVertex->mTexCoord = glm::vec2(1.0f, 1.0f);
//	//pVertex->mTexID = pTexInd;
//	pVertex++;
//
//	pVertex->mPos = glm::vec3(pPos.x, pPos.y + size, pPos.z);
//	pVertex->mColor = glm::vec4(pColor.x, pColor.y, pColor.z, pColor.w);
//	pVertex->mTexCoord = glm::vec2(0.0f, 1.0f);
//	//pVertex->mTexID = pTexInd;
//	pVertex++;
//
//	return pVertex;
//}
