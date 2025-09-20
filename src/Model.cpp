#include "Model.h"

Model::Model(const glm::vec3& pOriginPos, Material* pMaterialPtr,
			 const std::vector<Texture2>& pTextures,
		     std::vector<std::unique_ptr<Mesh>>& pMeshes)
{
	init(pOriginPos, pMaterialPtr, pTextures, pMeshes);
}

Model::Model(const glm::vec3& pOriginPos, Material* pMaterialPtr, 
			 const std::filesystem::path& pPath, const std::vector<Texture2>& pTextures)
{
	init(pOriginPos, pMaterialPtr, pPath, pTextures);
}

void Model::init(const glm::vec3& pOriginPos, Material* pMaterialPtr,
				 const std::vector<Texture2>& pTextures,
				 std::vector<std::unique_ptr<Mesh>>& pMeshes)
{
	mOriginPos = pOriginPos;
	mMaterial = pMaterialPtr;
	mTextures = pTextures;
	for (auto& i : pMeshes)
	{
		mMeshes.push_back(std::move(i));
	}
}

void Model::init(const glm::vec3& pOriginPos, Material* pMaterialPtr, 
				 const std::filesystem::path& pPath, const std::vector<Texture2>& pTextures)
{
	mOriginPos = pOriginPos;
	mPos = pOriginPos;
	mMaterial = pMaterialPtr;
	mTextures = pTextures;
	std::vector<Vertex> vertices = mOBJLoader.loadOBJ(pPath);
	std::vector<uint32_t> indices(vertices.size());
	for (size_t i = 0; i < indices.size(); ++i)
	{
		indices[i] = i;
	}
	mMeshes.push_back(std::make_unique<Mesh>(vertices, indices, pTextures));
}

void Model::initMVP(int32_t pWinWidth, int32_t pWinHeight, const glm::mat4& pProjMatrix,
					const glm::mat4& pViewMatrix, const glm::vec3& pTranslation, 
					const std::pair<float, glm::vec3>& pDegreeRotate, const glm::vec3& pScale)
{
	for (auto& i : mMeshes)
	{
		i->initMVP(pWinWidth, pWinHeight, pProjMatrix, pViewMatrix, pTranslation, pDegreeRotate, pScale);
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
	mMaterial->sendToShader(pShader);
}
