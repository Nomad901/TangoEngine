#pragma once
#include <iostream>
#include <format>
#include <filesystem>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Utils.h"

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices)
#define MAX_NUMBER_VERTICES_PER_BONE 4

class Animator
{
private:
	struct VertexBoneData;
public:
	Animator();
	Animator(const std::filesystem::path& pPath);

	void init(const std::filesystem::path& pPath);

private:
	void parseScene(const aiScene* pScene);
	void parseMeshes(const aiScene* pScene);
	void parseMeshBones(const aiMesh* pMesh);
	void parseSingleBone(uint32_t pIndex, const aiBone* pBone);
	
	int32_t getBonesIndex(const aiBone* pBone);

private:
	std::vector<VertexBoneData> mVertexBoneDataStorage;
	std::vector<int32_t> mMeshBaseVertices;
	std::unordered_map<std::string, uint32_t> mBonesIndices;
	
private:
	struct VertexBoneData
	{
	public:
		VertexBoneData() = default;

		void addBoneData(uint32_t pBoneId, float pBoneWeight);

	public:
		std::array<uint32_t, MAX_NUMBER_VERTICES_PER_BONE> mBonesId;
		std::array<float, MAX_NUMBER_VERTICES_PER_BONE> mWeights;
	};
};

