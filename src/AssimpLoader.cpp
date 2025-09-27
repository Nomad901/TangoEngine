#include "AssimpLoader.h"

AssimpLoader::AssimpLoader(const std::filesystem::path& pPath)
{
	loadModel(pPath);
}

void AssimpLoader::loadModel(const std::filesystem::path& pPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pPath.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << std::format("ERROR::ASSIMP: {}\n", importer.GetErrorString());
		return;
	}

	mDirectory = pPath.string().substr(0, pPath.string().find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

const std::vector<Vertex>& AssimpLoader::getVertices() const noexcept
{
	return mVertices;
}

const std::vector<uint32_t>& AssimpLoader::getIndices() const noexcept
{
	return mIndices;
}

std::vector<std::unique_ptr<Mesh>>& AssimpLoader::getMeshes() noexcept
{
	return mMeshes;
}

void AssimpLoader::processNode(aiNode* pNode, const aiScene* pScene)
{
	for (uint32_t i = 0; i < pNode->mNumMeshes; ++i)
	{
		aiMesh* mesh = pScene->mMeshes[pNode->mMeshes[i]];
		processMesh(mesh, pScene);
	}
	for (uint32_t i = 0; i < pNode->mNumChildren; ++i)
	{
		processNode(pNode->mChildren[i], pScene);
	}
}

void AssimpLoader::processMesh(aiMesh* pMesh, const aiScene* pScene)
{
	std::vector<Vertex> vertices;
	vertices.reserve(pMesh->mNumVertices);
	std::vector<uint32_t> indices;
	indices.reserve(pMesh->mNumVertices * 6);
	std::vector<Texture2> textures;
	textures.reserve(pMesh->mNumVertices * 3);

	for (uint32_t i = 0; i < pMesh->mNumVertices; ++i)
	{
		Vertex vertex;
		glm::vec3 vector;
		vector.x = pMesh->mVertices[i].x;
		vector.y = pMesh->mVertices[i].y;
		vector.z = pMesh->mVertices[i].z;
		vertex.mPos = vector;

		if (pMesh->HasNormals())
		{
			vector.x = pMesh->mNormals[i].x;
			vector.y = pMesh->mNormals[i].y;
			vector.z = pMesh->mNormals[i].z;
			vertex.mNormals = vector;
		}
		if (pMesh->mTextureCoords[0])
		{
			glm::vec2 vec2;
			vec2.x = pMesh->mTextureCoords[0][i].x;
			vec2.y = pMesh->mTextureCoords[0][i].y;
			vertex.mTexCoord = vec2;
		}
		else
			vertex.mTexCoord = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	mVertices = vertices;
	for (uint32_t i = 0; i < pMesh->mNumFaces; ++i)
	{
		aiFace face = pMesh->mFaces[i];
		for (uint32_t i = 0; i < face.mNumIndices; ++i)
		{
			indices.push_back(face.mIndices[i]);
		}
	}
	mIndices = indices;
	mMeshes.push_back(std::make_unique<Mesh>(mVertices, mIndices));
}