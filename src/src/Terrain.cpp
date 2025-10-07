#include "Terrain.h"

Terrain::Terrain(const glm::vec3& pPos, const glm::vec3& pSize, 
				 const std::vector<std::filesystem::path>& pTexturePaths,
				 const glm::mat4& pProj)
{
	init(pPos, pSize, pTexturePaths, pProj);
}

void Terrain::init(const glm::vec3& pPos, const glm::vec3& pSize,
				   const std::vector<std::filesystem::path>& pTexturePaths,
				   const glm::mat4& pProj)
{
	mProj = pProj;
	mPos = pPos;
	mSize = pSize;

	std::string resourcePath = RESOURCES_PATH;
	mShader.init(resourcePath + "Shaders/terrainVert.glsl", resourcePath + "Shaders/terrainFrag.glsl", 
				 resourcePath + "Shaders/terrainTCS.glsl", resourcePath + "Shaders/terrainTES.glsl");
	
	// initializing textures
	if (!pTexturePaths.empty())
		mTextureManager.pushTexture("MainTexture", std::make_unique<Texture2>(pTexturePaths[0], true));
	std::string name;
	for (size_t i = 1; i < pTexturePaths.size(); ++i)
	{
		mTextureManager.pushTexture(name + std::to_string(i), std::make_unique<Texture2>(pTexturePaths[i]));
	}
	
	// binding textures
	mShader.bind();
	uint32_t counter = 0;
	for (auto& [key, value] : mTextureManager.getStorageTextures())
	{
		value->bind(GL_TEXTURE_2D, counter);
		counter++;
	}
	mShader.setUniform1i("uHeightMap", 0);
	mShader.setUniform1i("uGrassTex", 1);
	mShader.unbind();

	float terrainWidth = 1000.0f;
	float terrainHeight = 1000.0f;
    mRez = 20;
    mVertices.reserve(mRez * mRez * 4); 
    for (uint32_t j = 0; j < mRez; ++j)      
    {
        for (uint32_t i = 0; i < mRez; ++i)   
        {
            float x0 = -terrainWidth / 2.0f + terrainWidth * i / (float)mRez;
			float x1 = -terrainWidth / 2.0f + terrainWidth * (i + 1) / (float)mRez;
			float z0 = -terrainHeight / 2.0f + terrainHeight * j / (float)mRez;
			float z1 = -terrainHeight / 2.0f + terrainHeight * (j + 1) / (float)mRez;
			
			float height00 = mHeightsGenerator.generateHeight(x0, z0);
			float height01 = mHeightsGenerator.generateHeight(x0, z1);
			float height10 = mHeightsGenerator.generateHeight(x1, z0);
			float height11 = mHeightsGenerator.generateHeight(x1, z1);
			//float height00 = 0.0f;
			//float height01 = 0.0f;
			//float height10 = 0.0f;
			//float height11 = 0.0f;

			float u0 = x0;
			float u1 = x1;
			float v0 = z0;
			float v1 = z1;

            Vertex vertex0;
            vertex0.mPos = glm::vec3(x0, height00, z0);
            vertex0.mTexCoord = glm::vec2(u0, v0);
            vertex0.mNormals = glm::vec3(0.0f, 1.0f, 0.0f);
            vertex0.mColor = glm::vec4(1.0f);
            mVertices.push_back(vertex0);
			
            Vertex vertex1;
            vertex1.mPos = glm::vec3(x1, height10, z0);
            vertex1.mTexCoord = glm::vec2(u1, v0);
            vertex1.mNormals = glm::vec3(0.0f, 1.0f, 0.0f);
            vertex1.mColor = glm::vec4(1.0f);
            mVertices.push_back(vertex1);

            Vertex vertex2;
			vertex2.mPos = glm::vec3(x0, height01, z1);
			vertex2.mTexCoord = glm::vec2(u0, v1);
			vertex2.mNormals = glm::vec3(0.0f, 1.0f, 0.0f);
			vertex2.mColor = glm::vec4(1.0f);
            mVertices.push_back(vertex2);

            Vertex vertex3;
            vertex3.mPos = glm::vec3(x1, height11, z1);
            vertex3.mTexCoord = glm::vec2(u1, v1);
            vertex3.mNormals = glm::vec3(0.0f, 1.0f, 0.0f);
            vertex3.mColor = glm::vec4(1.0f);
            mVertices.push_back(vertex3);
        }
    }
	mMesh.init(mVertices);
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

TextureManager& Terrain::getTextureManager() noexcept
{
	return mTextureManager;
}

Mesh& Terrain::getMesh() noexcept
{
	return mMesh;
}

HeightsGenerator& Terrain::getHeightsGenerator() noexcept
{
	return mHeightsGenerator;
}

void Terrain::render(const glm::mat4& pViewMatrix)
{
	glDisable(GL_CULL_FACE);
	mView = pViewMatrix;
	updateUniforms();
	mMesh.getVAO().bind();
	uint32_t counter = 0;
	for (auto& [key, value] : mTextureManager.getStorageTextures())
	{
		value->bind(GL_TEXTURE_2D, counter);
		counter++;
	}
	glDrawArrays(GL_PATCHES, 0, 4 * mRez * mRez);
	glEnable(GL_CULL_FACE);
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