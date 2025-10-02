#include "Model.h"

Model::Model(const glm::vec3& pOriginPos, std::vector<std::unique_ptr<Mesh>>& pMeshes)
{
	init(pOriginPos, pMeshes);
}

Model::Model(const glm::vec3& pOriginPos, const std::filesystem::path& pPath)
{
	std::string extension = pPath.extension().string();
	if (extension == ".obj")
		initOBJmodel(pOriginPos, pPath);
	else
		init(pOriginPos, pPath);
}

void Model::init(const glm::vec3& pOriginPos, std::vector<std::unique_ptr<Mesh>>& pMeshes)
{
	mOriginPos = pOriginPos;
	for (auto& i : pMeshes)
	{
		mMeshes.push_back(std::move(i));
	}
}

void Model::init(const glm::vec3& pOriginPos, const std::filesystem::path& pPath)
{
	mOriginPos = pOriginPos;
	mPos = pOriginPos;
	mAssimpLoader.loadModel(pPath);
	mMeshes = std::move(mAssimpLoader.getMeshes());
}

void Model::initOBJmodel(const glm::vec3& pOriginPos, const std::filesystem::path& pPath)
{
	mOriginPos = pOriginPos;
	mPos = pOriginPos;
	std::vector<Vertex> vertices = mOBJLoader.loadOBJ(pPath);
	std::vector<uint32_t> indices(vertices.size());
	for (size_t i = 0; i < indices.size(); ++i)
	{
		indices[i] = i;
	}
	mMeshes.push_back(std::make_unique<Mesh>(vertices, indices));
}

void Model::setInstancedData(const std::vector<glm::mat4>& pMatrices, GLenum pUsage)
{
	for (auto& i : mMeshes)
	{
		i->initInstancedData(pMatrices, pUsage);
	}
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

std::vector<std::unique_ptr<Mesh>>& Model::getMeshes() noexcept
{
	return mMeshes;
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

void Model::render(Shader& pShader, const glm::vec3& pColor)
{
	updateUniforms(pShader, pColor);
	for (auto& i : mMeshes)
	{
		i->drawModel();
	}
}

void Model::renderInstanced(uint32_t pNumber)
{
	for (auto& i : mMeshes)
	{
		i->drawInstances(pNumber);
	}
}

void Model::updateUniforms(Shader& pShader, const glm::vec3& pColor)
{
	pShader.setUniform3fv("uObjectColor", pColor);
	pShader.setMatrixUniform4fv("uModel", getModelMatrix());
	pShader.setMatrixUniform4fv("uMVP", getMVP());
}
