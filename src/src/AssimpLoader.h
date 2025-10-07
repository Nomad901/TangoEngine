#pragma once
#include <iostream>
#include <format>
#include <vector>
#include <filesystem>
#include <string>
#include <memory>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "VBO.h"
#include "Mesh.h"
#include "Texture2.h"

class AssimpLoader
{
public:
	AssimpLoader() = default;
	AssimpLoader(const std::filesystem::path& pPath);

	void loadModel(const std::filesystem::path& pPath);

	const std::vector<Vertex>& getVertices() const noexcept;
	const std::vector<uint32_t>& getIndices() const noexcept;
	std::vector<std::unique_ptr<Mesh>>& getMeshes() noexcept;

private:
	void processNode(aiNode* pNode, const aiScene* pScene);
	void processMesh(aiMesh* pMesh, const aiScene* pScene);

private:
	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndices;
	std::vector<std::unique_ptr<Mesh>> mMeshes;
	std::string mDirectory;

};

