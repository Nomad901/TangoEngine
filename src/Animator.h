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

#define MAX_NUMBER_VERTICES_PER_BONE 4

class Animator
{
private:

public:
	//Animator();
	//Animator(const std::filesystem::path& pPath);
	//
	//void init(const std::filesystem::path& pPath);

private:
	void parseScene(const aiScene* pScene);
	void parseMeshes(const aiScene* pScene);
	void parseMeshBones(uint32_t pIndex, const aiMesh* pMesh);
	//void parseSingleBone(uint32_t pIndex, const aiBone* pBone);

private:
	//std::vector<VertexBoneData> mVertexBoneDataStorage;
	std::vector<int32_t> mMeshBaseVertices;
};

