#include "Model.h"

Model::Model(const glm::vec3& pOriginPos,
		     const std::pair<Texture2&, Texture2&>& pTextures, 
				   std::pair<uint32_t, uint32_t> pSlots,
				   std::vector<std::unique_ptr<Mesh>>& pMeshes)
{
	init(pOriginPos, pTextures, pSlots, pMeshes);
}

Model::Model(const glm::vec3& pOriginPos, const std::filesystem::path& pPath, 
			 const std::pair<Texture2&, Texture2&>& pTextures,
			 std::pair<uint32_t, uint32_t> pSlots)
{
	init(pOriginPos, pPath, pTextures, pSlots);
}

void Model::init(const glm::vec3& pOriginPos,
				 const std::pair<Texture2&, Texture2&>& pTextures,
					   std::pair<uint32_t, uint32_t> pSlots,
					   std::vector<std::unique_ptr<Mesh>>& pMeshes)
{
	mOriginPos = pOriginPos;
	mTextures.first = std::move(pTextures.first);
	mTextures.second = std::move(pTextures.second);
	mSlots = pSlots;
	for (auto& i : pMeshes)
	{
		mMeshes.push_back(std::move(i));
	}
}

void Model::init(const glm::vec3& pOriginPos, const std::filesystem::path& pPath,
				 const std::pair<Texture2&, Texture2&>& pTextures,
					   std::pair<uint32_t, uint32_t> pSlots)
{
	mOriginPos = pOriginPos;
	mPos = pOriginPos;
	mTextures.first = std::move(pTextures.first);
	mTextures.second = std::move(pTextures.second);
	mSlots = pSlots;
	std::vector<Vertex> vertices = mOBJLoader.loadOBJ(pPath);
	std::vector<uint32_t> indices(vertices.size());
	for (size_t i = 0; i < indices.size(); ++i)
	{
		indices[i] = i;
	}
	mMeshes.push_back(std::make_unique<Mesh>(vertices, indices));
}

void Model::initMVP(const glm::mat4& pProjMatrix,const glm::mat4& pViewMatrix, const glm::vec3& pTranslation,
					const std::pair<float, glm::vec3>& pDegreeRotate, const glm::vec3& pScale)
{
	for (auto& i : mMeshes)
	{
		i->initMVP(pProjMatrix, pViewMatrix, pTranslation, pDegreeRotate, pScale);
	}
}

void Model::initMVP(const glm::mat4& pProjMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pModel)
{
	for (auto& i : mMeshes)
	{
		i->initMVP(pProjMatrix, pViewMatrix, pModel);
	}
}

glm::mat4 Model::getModelMatrix() const noexcept
{
	return mMeshes[0]->getModelMatrix();
}

glm::mat4 Model::getViewMatrix() const noexcept
{
	return mMeshes[0]->getViewMatrix();
}

glm::mat4 Model::getProjMatrix() const noexcept
{
	return mMeshes[0]->getProjMatrix();
}

glm::mat4 Model::getMVP()
{
	return mMeshes[0]->getMVP();
}

void Model::setUniforms(Shader& pShader, const glm::vec4& pColor)
{
	for (auto& i : mMeshes)
	{
		i->setUniforms(pShader, pColor);
	}
}

void Model::setPos(const glm::vec3& pPos)
{
	mMeshes[0]->setPos(pPos);
	mPos = pPos;
}

void Model::setSize(const glm::vec3& pSize)
{
	mMeshes[0]->setSize(pSize);
	mSize = pSize;
}

glm::vec3 Model::getPos() const noexcept
{
	return mPos;
}

glm::vec3 Model::getSize() const noexcept
{
	return mSize;
}

glm::vec3 Model::getOriginPos() const noexcept
{
	return mOriginPos;
}

Texture2& Model::getFirstTex() noexcept
{
	return mTextures.first;
}

Texture2& Model::getSecondTex() noexcept
{
	return mTextures.second;
}

std::pair<uint32_t, uint32_t> Model::getSlots() const noexcept
{
	return mSlots;
}

void Model::takeModel(bool pTake)
{
	mIsTaken = pTake;
}

bool Model::modelIsTaken() const noexcept
{
	return mIsTaken;
}

void Model::render(Shader& pShader)
{
	pShader.bind();
	updateUniforms(pShader);
	for (auto& i : mMeshes)
	{
		i->draw();
	}
}

void Model::render()
{
	for (auto& i : mMeshes)
	{
		i->draw();
	}
}

void Model::updateUniforms(Shader& pShader)
{
	pShader.setMatrixUniform4fv("uModel", getModelMatrix());
	pShader.setMatrixUniform4fv("uMVP", getMVP());
}
