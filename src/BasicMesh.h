#pragma once
#include <concepts>

#include "Log.h"
#include "VAO.h"
#include "VBO.h"
#include "VBOLayout.h"
#include "EBO.h"
#include "PBRMaterial.h"
#include "Transform.h"

#include "assimp/Importer.hpp"

// The difference between Mesh.h and BasicMesh.h:
// If u wanna load a model, you need to use Model class, which 
// contains Mesh class. So, basically Model class accepts only obj files,
// but can also accepts other files through assimp (but obj is faster).
// This class - can accept any type of model, it will convert the model in place
// without calling other classes and also it will load materials. 
// Basically this is almost like skinnedMesh, but skinnedMesh is focused on bones;

class BasicMesh
{
public:
	enum class vertexFormat : uint32_t
	{
		vertexSOA = 0,
		vertexAOS = 1,
		AUTO
	};
public:
	BasicMesh() = default;
	~BasicMesh() = default;

	virtual void loadMesh(const std::filesystem::path& pPath) = 0;
	virtual void destroy() = 0;

	virtual uint32_t getNumberOfVertices() const noexcept = 0;

	uint32_t getNumberOfMeshes() const noexcept;
	uint32_t getNumberOfIndices() const noexcept;
	
	const Transform& getTransform() const noexcept;
	Transform& getTransform() noexcept;
	void setTransform(const Transform& pTransform);
	
	const PBRMaterial& getMaterial() const noexcept;
	PBRMaterial& getMaterial() noexcept;

	virtual vertexFormat getVertexFormat() const noexcept = 0;
	
	virtual void render() = 0;

protected:
	struct basicMeshStruct
	{
		uint32_t mNumberOfVertices{};
		uint32_t mBaseVertex{};
		uint32_t mBaseIndex{};
		uint32_t mMaterialIndex{};
	};
	std::vector<basicMeshStruct> mMeshes;
	std::vector<uint32_t> mIndices;

	Transform mTransform;
	PBRMaterial mMaterial;

	Assimp::Importer mImporter;
	const aiScene* mScene{ nullptr };	
};