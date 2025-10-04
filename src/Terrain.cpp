#include "Terrain.h"

Terrain::Terrain(const glm::vec3& pPos, const glm::vec3& pSize, 
				 const std::filesystem::path& pTexturePath,
				 const glm::mat4& pModel, const glm::mat4& pProj,
				 const glm::mat4& pView)
{
	init(pPos, pSize, pTexturePath, pModel, pProj, pView);
}

void Terrain::init(const glm::vec3& pPos, const glm::vec3& pSize,
				   const std::filesystem::path& pTexturePath, 
				   const glm::mat4& pModel, const glm::mat4& pProj,
				   const glm::mat4& pView)
{
	mModel = pModel;
	mProj = pProj;
	mView = pView;
	mPos = pPos;
	mSize = pSize;
	std::string resourcePath = RESOURCES_PATH;
	mShader.init(resourcePath + "Shaders/terrainVert.glsl", resourcePath + "Shaders/terrainFrag.glsl");
	mTexture.init(pTexturePath);
	mNumStrips = mTexture.getHeight() - 1;
	mNumVertsPerStrip = mTexture.getWidth() * 2;

	mVAO.bind();
	mVBO.init(mTexture.getVertices().data(), mTexture.getVertices().size() * sizeof(glm::vec3), GL_STATIC_DRAW);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mEBO.init(mTexture.getIndices().data(), mTexture.getIndices().size());
}

void Terrain::setPos(const glm::vec3& pPos)
{
	mPos = pPos;
}

void Terrain::setSize(const glm::vec3& pSize)
{
	mSize = pSize;
}

void Terrain::setModel(const glm::mat4& pModel)
{
	mModel = pModel;
}

void Terrain::setProj(const glm::mat4& pProj)
{
	mProj = pProj;
}

void Terrain::setView(const glm::mat4& pView)
{
	mView = pView;
}

const glm::vec3& Terrain::getPos() const noexcept
{
	return mPos;
}

const glm::vec3& Terrain::getSize() const noexcept
{
	return mSize;
}

Texture2& Terrain::getTexture() noexcept
{
	return mTexture;
}

void Terrain::render()
{
	updateUniforms();
	mVAO.bind();
	for (uint32_t strip = 0; strip < mNumStrips; ++strip)
	{
		glDrawElements(GL_TRIANGLE_STRIP, mNumVertsPerStrip, GL_UNSIGNED_INT,
					  (void*)(sizeof(uint32_t) * mNumVertsPerStrip * strip));
	}
}

void Terrain::updateUniforms()
{
	mShader.bind();
	glm::mat4 MVP = mProj * mView * mModel;
	mShader.setMatrixUniform4fv("uMVP", MVP);
	mShader.setMatrixUniform4fv("uView", mView);
	mShader.setMatrixUniform4fv("uModel", mModel);
}
