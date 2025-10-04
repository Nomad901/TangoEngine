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
	mShader.init(resourcePath + "Shaders/terrainVert.glsl", resourcePath + "Shaders/terrainFrag.glsl", 
				 resourcePath + "Shaders/terrainTCS.glsl", resourcePath + "Shaders/terrainTES.glsl");
	mTexture.initMipMap(pTexturePath);
	mShader.bind();
	mTexture.bind(GL_TEXTURE_2D);
	mShader.setUniform1i("uHeightMap", 0);
	mShader.unbind();

    mRez = 20;
    mVertices.reserve(mRez * mRez * 4); 
    for (uint32_t j = 0; j < mRez; ++j)      
    {
        for (uint32_t i = 0; i < mRez; ++i)   
        {
            float x0 = -mTexture.getWidth() / 2.0f + mTexture.getWidth() * i / (float)mRez;
            float x1 = -mTexture.getWidth() / 2.0f + mTexture.getWidth() * (i + 1) / (float)mRez;
            float z0 = -mTexture.getHeight() / 2.0f + mTexture.getHeight() * j / (float)mRez;
            float z1 = -mTexture.getHeight() / 2.0f + mTexture.getHeight() * (j + 1) / (float)mRez;

            float u0 = i / (float)mRez;
            float u1 = (i + 1) / (float)mRez;
            float v0 = j / (float)mRez;
            float v1 = (j + 1) / (float)mRez;

            Vertex vertex0;
            vertex0.mPos = glm::vec3(x0, 0.0f, z0);
            vertex0.mTexCoord = glm::vec2(u0, v0);
            vertex0.mNormals = glm::vec3(0.0f, 1.0f, 0.0f);
            vertex0.mColor = glm::vec4(1.0f);
            mVertices.push_back(vertex0);

            // Vertex 1: bottom-right  
            Vertex vertex1;
            vertex1.mPos = glm::vec3(x1, 0.0f, z0);
            vertex1.mTexCoord = glm::vec2(u1, v0);
            vertex1.mNormals = glm::vec3(0.0f, 1.0f, 0.0f);
            vertex1.mColor = glm::vec4(1.0f);
            mVertices.push_back(vertex1);

            // Vertex 2: top-left
            Vertex v2;
            v2.mPos = glm::vec3(x0, 0.0f, z1);
            v2.mTexCoord = glm::vec2(u0, v1);
            v2.mNormals = glm::vec3(0.0f, 1.0f, 0.0f);
            v2.mColor = glm::vec4(1.0f);
            mVertices.push_back(v2);

            // Vertex 3: top-right
            Vertex v3;
            v3.mPos = glm::vec3(x1, 0.0f, z1);
            v3.mTexCoord = glm::vec2(u1, v1);
            v3.mNormals = glm::vec3(0.0f, 1.0f, 0.0f);
            v3.mColor = glm::vec4(1.0f);
            mVertices.push_back(v3);
        }
    }

	mVAO.generate();
	mVAO.bind();
	mVBO.init(mVertices, GL_STATIC_DRAW);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 4);
	mVBOLayout.pushLayout(GL_FLOAT, 2);
	mVAO.addBuffer(mVBO, mVBOLayout);
	glPatchParameteri(GL_PATCH_VERTICES, 4);

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
	glDrawArrays(GL_PATCHES, 0, 4 * mRez * mRez);
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