#include "Animator.h"

void Animator::parseScene(const aiScene* pScene)
{
	parseMeshes(pScene);
}

void Animator::parseMeshes(const aiScene* pScene)
{
	uint32_t numVerticesTotal = 0;
	uint32_t numIndicesTotal  = 0;
	uint32_t numBonesTotal    = 0;

	mMeshBaseVertices.resize(pScene->mNumMeshes);

	for (size_t i = 0; i < pScene->mNumMeshes; ++i)
	{
		const aiMesh* mesh = pScene->mMeshes[i];
		uint32_t numVertices = mesh->mNumVertices;
		uint32_t numIndices  = mesh->mNumFaces * 3;
		uint32_t numBones    = mesh->mNumBones;
		mMeshBaseVertices[i] = numVerticesTotal;
		numVerticesTotal += numVertices;
		numIndicesTotal  += numIndices;
		numBonesTotal    += numBones;

		//mVertexBoneDataStorage.resize(numBonesTotal);
		if (mesh->HasBones())
			parseMeshBones(i, mesh);
	}
}

void Animator::parseMeshBones(uint32_t pIndex, const aiMesh* pMesh)
{
	for (size_t i = 0; i < pMesh->mNumBones; ++i)
	{
		//parseSingleBone(pIndex, pMesh->mBones[i]);
	}
}