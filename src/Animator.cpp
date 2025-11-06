#include "Animator.h"

Animator::Animator()
{
	mVertexBoneDataStorage.reserve(10000);
	mMeshBaseVertices.reserve(10000);
	mBonesIndices.reserve(10000);
}

Animator::Animator(const std::filesystem::path& pPath)
{
	mVertexBoneDataStorage.reserve(10000);
	mMeshBaseVertices.reserve(10000);
	mBonesIndices.reserve(10000);

	init(pPath);
}

void Animator::init(const std::filesystem::path& pPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pPath.string(), ASSIMP_LOAD_FLAGS);
	if (!scene)
	{
		std::cout << std::format("Couldnt load the scene with this path: {}\n", pPath.string());
		return;
	}
	parseScene(scene);
}

void Animator::parseScene(const aiScene* pScene)
{
	parseMeshes(pScene);
}

void Animator::parseMeshes(const aiScene* pScene)
{
	uint32_t numVerticesTotal = 0;
	uint32_t numIndicesTotal  = 0;
	uint32_t numBonesTotal    = 0;

	for (size_t i = 0; i < pScene->mNumMeshes; ++i)
	{
		const aiMesh* mesh = pScene->mMeshes[i];
		uint32_t numVertices = mesh->mNumVertices;
		uint32_t numIndices  = mesh->mNumFaces * 3;
		uint32_t numBones    = mesh->mNumBones;
		numVerticesTotal += numVertices;
		numIndicesTotal  += numIndices;
		numBonesTotal    += numBones;

		if (mesh->HasBones())
			parseMeshBones(mesh);
	}
}

void Animator::parseMeshBones(const aiMesh* pMesh)
{
	for (size_t i = 0; i < pMesh->mNumBones; ++i)
	{
		parseSingleBone(i, pMesh->mBones[i]);
	}
}

void Animator::parseSingleBone(uint32_t pIndex, const aiBone* pBone)
{
	for (size_t i = 0; i < pBone->mNumWeights; ++i)
	{
		const aiVertexWeight& vertexWeight = pBone->mWeights[i];
	}
}

int32_t Animator::getBonesIndex(const aiBone* pBone)
{
	int32_t boneId = 0;
	std::string boneName = pBone->mName.C_Str();

	if (mBonesIndices.contains(boneName))
	{
		boneId = mBonesIndices[boneName];
	}
	else
	{
		boneId = mBonesIndices.size();
		mBonesIndices.emplace(boneName, boneId);
	}

	return boneId;
}

void Animator::VertexBoneData::addBoneData(uint32_t pBoneId, float pBoneWeight)
{
	uint32_t counter = 0;
	for (size_t i = 0; i < mBonesId.size(); ++i)
	{
		if (mWeights[i] == 0.0f)
		{
			mBonesId[i] = pBoneId;
			mWeights[i] = pBoneWeight;
			return;
		}
		counter++;
	}
	std::cout << std::format("We have more bones than we can dissect: {}!\n", counter);
}
