#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& pVertices, 
		   const std::vector<uint32_t>& pIndices, 
		   const std::vector<Texture2>& pTexture)
{
	init(pVertices, pIndices, pTexture);
}

Mesh::Mesh(const std::vector<Vertex>& pVertices, const std::vector<uint32_t>& pIndices)
{
	init(pVertices, pIndices);
}

Mesh::Mesh(const std::weak_ptr<Primitive>& pPrimitive,
		   const std::vector<Texture2>& pTexture)
{
	init(pPrimitive, pTexture);
}

Mesh::Mesh(const std::weak_ptr<Primitive>& pPrimitive)
{
	init(pPrimitive);
}

void Mesh::init(const std::vector<Vertex>& pVertices,
				const std::vector<uint32_t>& pIndices,
				const std::vector<Texture2>& pTextures)
{
	mPrimitive = std::make_unique<Primitive>();
	
	mVAO.generate();
	mVAO.bind();
	mVBO.init(pVertices, GL_STATIC_DRAW);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 4);
	mVBOLayout.pushLayout(GL_FLOAT, 2);
	mVAO.addBuffer(mVBO, mVBOLayout);
	mEBO.init(pIndices.data(), pIndices.size());
	mTextures = pTextures;
	mPrimitive->setVertexStrg(pVertices);
	mPrimitive->setIndexStrg(pIndices);
}

void Mesh::init(const std::vector<Vertex>& pVertices, const std::vector<uint32_t>& pIndices)
{
	mPrimitive = std::make_unique<Primitive>();

	mVAO.generate();
	mVAO.bind();
	mVBO.init(pVertices, GL_STATIC_DRAW);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 4);
	mVBOLayout.pushLayout(GL_FLOAT, 2);
	mVAO.addBuffer(mVBO, mVBOLayout);
	mEBO.init(pIndices.data(), pIndices.size());
	mPrimitive->setVertexStrg(pVertices);
	mPrimitive->setIndexStrg(pIndices);
}

void Mesh::init(const std::weak_ptr<Primitive>& pPrimitive,
				const std::vector<Texture2>& pTextures)
{
	if (std::shared_ptr primitive = pPrimitive.lock())
	{
		mPrimitive = primitive;

		mVAO.generate();
		mVAO.bind();
		mVBO.init(mPrimitive->getVertexStrg(), GL_STATIC_DRAW);
		mVBOLayout.pushLayout(GL_FLOAT, 3);
		mVBOLayout.pushLayout(GL_FLOAT, 3);
		mVBOLayout.pushLayout(GL_FLOAT, 4);
		mVBOLayout.pushLayout(GL_FLOAT, 2);
		mVAO.addBuffer(mVBO, mVBOLayout);
		mEBO.init(mPrimitive->getIndexStrg().data(), mPrimitive->getIndexStrg().size());
		mTextures = pTextures;
	}
}

void Mesh::init(const std::weak_ptr<Primitive>& pPrimitive)
{
	if (std::shared_ptr primitive = pPrimitive.lock())
	{
		mPrimitive = primitive;
		mTexture = mPrimitive->getTexture();

		mVAO.generate();
		mVAO.bind();
		mVBO.init(mPrimitive->getVertexStrg(), GL_STATIC_DRAW);
		mVBOLayout.pushLayout(GL_FLOAT, 3);
		mVBOLayout.pushLayout(GL_FLOAT, 3);
		mVBOLayout.pushLayout(GL_FLOAT, 4);
		mVBOLayout.pushLayout(GL_FLOAT, 2);
		mVAO.addBuffer(mVBO, mVBOLayout);
		mEBO.init(mPrimitive->getIndexStrg().data(), mPrimitive->getIndexStrg().size());
	}
}

void Mesh::initMVP(int32_t pWinWidth, int32_t pWinHeight, const glm::mat4 pProjMatrix, 
				   const glm::mat4& pViewMatrix,
				   const glm::vec3& pTranslation, const std::pair<float, glm::vec3>& pDegreeRotate,
				   const glm::vec3& pScale)
{
	mProjMatrix = glm::mat4(1.0f);
	mModelMatrix = glm::mat4(1.0f);
	mRotate = pDegreeRotate;
	mPos = pTranslation;
	mSize = pScale;
	mProjMatrix = pProjMatrix;
	mViewMatrix = pViewMatrix;

	mModelMatrix = glm::rotate(mModelMatrix, glm::radians(pDegreeRotate.first), pDegreeRotate.second);
	mModelMatrix = glm::translate(mModelMatrix, pTranslation);
	mModelMatrix = glm::scale(mModelMatrix, pScale);

	mMVP = mProjMatrix * mViewMatrix * mModelMatrix;
}

void Mesh::setModelMatrix(const glm::mat4& pModelMatrix)
{
	mModelMatrix = pModelMatrix;
}

void Mesh::setViewMatrix(const glm::mat4& pViewMatrix)
{
	mViewMatrix = pViewMatrix;
}

void Mesh::setProjMatrix(const glm::mat4& pProjMatrix)
{
	mProjMatrix = pProjMatrix;
}

void Mesh::setMVP(const glm::mat4& pModelMatrix, 
				  const glm::mat4& pViewMatrix, 
				  const glm::mat4& pProjMatrix)
{
	mModelMatrix = pModelMatrix;
	mViewMatrix = pViewMatrix;
	mProjMatrix = pProjMatrix;
	mMVP = mProjMatrix * mViewMatrix * mModelMatrix;
}

void Mesh::recomputeMVP()
{
	mMVP = mProjMatrix * mViewMatrix * mModelMatrix;
}

void Mesh::setVertices(const std::vector<Vertex>& pVertices)
{
	mPrimitive->setVertexStrg(pVertices);
}

void Mesh::setIndices(const std::vector<uint32_t>& pIndices)
{
	mPrimitive->setIndexStrg(pIndices);
}

void Mesh::setTextures(const std::vector<Texture2>& pTextures)
{
	mTextures = pTextures;
}

std::vector<Vertex>& Mesh::getVertices()
{
	return mPrimitive->getVertexStrg();
}

std::vector<uint32_t>& Mesh::getIndices()
{
	return mPrimitive->getIndexStrg();
}

std::vector<Texture2>& Mesh::getTextures()
{
	return mTextures;
}

Primitive& Mesh::getPrimitive()
{
	return *mPrimitive.get();
}

void Mesh::setPos(const glm::vec3& pPos)
{
	mPos = pPos;
	rebuildMatrix();
}

void Mesh::setSize(const glm::vec3& pSize)
{
	mSize = pSize;
	rebuildMatrix();
}

const glm::mat4& Mesh::getModelMatrix() const noexcept
{
	return mModelMatrix;
}

const glm::mat4& Mesh::getViewMatrix() const noexcept
{
	return mViewMatrix;
}

const glm::mat4& Mesh::getProjMatrix() const noexcept
{
	return mProjMatrix;
}

const glm::mat4& Mesh::getMVP()
{
	return mMVP;
}

const glm::vec3& Mesh::getPos() const noexcept
{
	return mPos;
}

const glm::vec3& Mesh::getSize() const noexcept
{
	return mSize;
}

const VAO&Mesh::getVAO() const noexcept
{
	return mVAO;
}

const VBO& Mesh::getVBO() const noexcept
{
	return mVBO;
}

const EBO& Mesh::getEBO() const noexcept
{
	return mEBO;
}

const VBOLayout& Mesh::getVBOLayout() const noexcept
{
	return mVBOLayout;
}

void Mesh::setUniforms(Shader& pShader, const glm::vec3& pColor)
{
	pShader.setUniform3fv("uObjectColor", pColor);
	pShader.setMatrixUniform4fv("uModel", getModelMatrix());
	pShader.setMatrixUniform4fv("uMVP", getMVP());
}

void Mesh::takeMesh(bool pTake)
{
	mIsTaken = pTake;
}

bool Mesh::meshIsTaken() const noexcept
{
	return mIsTaken;
}

void Mesh::draw()
{
	//for (size_t i = 0; i < mTextures.size(); ++i)
	//{
	//	mTextures[i].bind(i);
	//}
	mTexture.bind(mPrimitive->getTexSloth());
	mVAO.bind();
	glDrawElements(GL_TRIANGLES, mEBO.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Mesh::drawInFrameBuffer(Texture2& pTexture)
{
	mVAO.bind();
	glDisable(GL_DEPTH_TEST);
	pTexture.bind();
	glDrawElements(GL_TRIANGLES, mEBO.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Mesh::rebuildMatrix()
{
	mModelMatrix = glm::mat4(1.0f);
	mModelMatrix = glm::rotate(mModelMatrix, glm::radians(mRotate.first), mRotate.second);
	mModelMatrix = glm::translate(mModelMatrix, mPos);
	mModelMatrix = glm::translate(mModelMatrix, mSize);
}
