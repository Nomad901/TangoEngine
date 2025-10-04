#include "Terrain.h"

Terrain::Terrain(const glm::vec3& pPos, const glm::vec3& pSize, 
				 const std::filesystem::path& pTexturePath,
				 const glm::mat4& pProj)
{
	init(pPos, pSize, pTexturePath, pProj);
}

void Terrain::init(const glm::vec3& pPos, const glm::vec3& pSize,
				   const std::filesystem::path& pTexturePath, 
				   const glm::mat4& pProj)
{
	mProj = pProj;
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
	mVAO.addBuffer(mVBO, mVBOLayout);
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

void Terrain::setProj(const glm::mat4& pProj)
{
	mProj = pProj;
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

void Terrain::render(const glm::mat4& pViewMatrix)
{
	mView = pViewMatrix;
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
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, mPos);
	model = glm::scale(model, mSize);

	glm::mat4 MVP = mProj * mView * model;
	mShader.setMatrixUniform4fv("uMVP", MVP);
	mShader.setMatrixUniform4fv("uView", mView);
	mShader.setMatrixUniform4fv("uModel", model);
}