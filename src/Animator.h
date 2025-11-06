#pragma once
#include <iostream>
#include <format>
#include <filesystem>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices)

class Animator
{
public:
	Animator() = default;
	Animator(const std::filesystem::path& pPath);

	void init(const std::filesystem::path& pPath);

private:
	void parseScene(const aiScene* pScene);
	void parseMeshes(const aiScene* pScene);
	void parseMeshBones(const aiMesh* pMesh);
	void parseSingleBone(uint32_t pIndex, const aiBone* pBone);

private:

};

