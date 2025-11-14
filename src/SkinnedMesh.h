#pragma once
#include <iostream>
#include <format>
#include <filesystem>

#include "glad/glad.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/glm.hpp"

#include "Utils.h"
#include "Transform.h" 
#include "PBRMaterial.h"
#include "Texture2.h"
#include "Shader.h"
#include "VAO.h"
#include "EBO.h"
#include "Timer.h"

#define MAX_NUMBER_BONES_PER_VERTEX 8
#define INVALID_MATERIAL 0xFFFFFFFF

class SkinnedMesh
{
private: 
	enum class BUFFER_TYPE : uint32_t;
	enum class IndexType;
public:
	SkinnedMesh() = default;
	~SkinnedMesh();

	void loadMesh(const std::filesystem::path& pPath);
	void destroy();

	uint32_t getNumBones() const;
	uint32_t getNumVertices() const;
	uint32_t getNumIndices() const;
	Transform& getTransform() noexcept;
	PBRMaterial& getMaterial() noexcept;
	void getBoneTransformations(float pAnimTime, std::vector<glm::mat4>& pTransformations);

	void render();

private:
	void initFromSceneAssimp(const aiScene* pScene, const std::filesystem::path& pPath);
	void populateBuffers();

	std::pair<uint32_t, uint32_t> getNumVerticesAndIndices(const aiScene* pScene);

	void initAllMeshes(const aiScene* pScene);
	void initSingleMesh(uint32_t pIndex, const aiMesh* pMesh);
	
	void initMaterials(const aiScene* pScene, const std::filesystem::path& pPath);
	
	void loadTexture(const std::filesystem::path& pPath, const aiMaterial* pMaterial, uint32_t pIndex);
	void loadDiffuseTexture(const std::filesystem::path& pPath, const aiMaterial* pMaterial, uint32_t pIndex);
	void loadSpecularTexture(const std::filesystem::path& pPath, const aiMaterial* pMaterial, uint32_t pIndex);
	void loadColors(const aiMaterial* pMaterial, uint32_t pIndex);

	void loadMeshBones(uint32_t pIndex, const aiMesh* pMesh);
	void loadSingleBone(uint32_t pIndex, const aiBone* pBone);
	int32_t getBonesIndex(const aiBone* pBone);
	
	uint32_t getIndexBufferType(BUFFER_TYPE pBUFFER_TYPE);

	void readNodeHierachy(float pAnimTime, const aiNode* pNode, const glm::mat4& pTransformation);
	const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, std::string_view pNodeName);

	void calcInterpolatedScale(aiVector3D& pScaling, float pAnimTimeTicks, const aiNodeAnim* pAiNodeAnim);
	void calcInterpolatedRotation(aiQuaternion& pRotation, float pAnimTimeTicks, const aiNodeAnim* pAiNodeAnim);
	void calcInterpolatedPosition(aiVector3D& pPosition, float pAnimTimeTicks, const aiNodeAnim* pAiNodeAnim);
	uint32_t findInterpolatedIndex(IndexType pIndexType, float pAnimTicks, const aiNodeAnim* pAiNodeAnim);

private:
	struct VertexBoneData
	{
	public:
		VertexBoneData() = default;

		void addBoneData(uint32_t pBoneId, float pBoneWeight);

	public:
		std::array<uint32_t, MAX_NUMBER_BONES_PER_VERTEX> mBonesId;
		std::array<float, MAX_NUMBER_BONES_PER_VERTEX> mWeights;
	};
	struct boneInfo
	{
		boneInfo(const glm::mat4& pOffset)
		{
			mOffset = pOffset;
			mTransformation = glm::mat4(0.0f);
		}

		glm::mat4 mOffset;
		glm::mat4 mTransformation;
	};
	struct basicMeshEntry
	{
		uint32_t mNumIndices{};
		uint32_t mBaseVertex{};
		uint32_t mBaseIndex{};	
		uint32_t mMaterialIndex{ INVALID_MATERIAL };
	};

	enum class BUFFER_TYPE : uint32_t
	{
		INDEX_BUFFER     = 0,
		POS_BUFFER       = 1,
		TEXCOORD_BUFFER  = 2,
		NORMAL_BUFFER    = 3, 
		BONES_BUFFER     = 4,
		NUM_TYPE_BUFFERS = 5
	};
	enum class IndexType
	{
		SCALING_INDEX  = 0,
		ROTATION_INDEX = 1,
		POSITION_INDEX = 2
	};

private:
	Transform mTransform;
	glm::mat4 mGlobalInverseTransf;
	Timer mTimer;

	Assimp::Importer mImporter;
	const aiScene* mScene{ nullptr };

	VAO mVAO;
	EBO mEBO;
	std::array<VBO, static_cast<uint32_t>(BUFFER_TYPE::NUM_TYPE_BUFFERS)> mBuffers;
	
	std::vector<basicMeshEntry> mMeshes;
	std::vector<PBRMaterial> mMaterials;

	std::vector<glm::vec3> mPos;
	std::vector<glm::vec3> mNormals;
	std::vector<glm::vec2> mTexCoord;
	std::vector<uint32_t> mIndices;
	std::vector<VertexBoneData> mBones;

	std::vector<boneInfo> mBonesInfo;

	std::unordered_map<std::string, uint32_t> mBonesIndices;
	std::unordered_map<std::string, const aiNodeAnim*> mStorageNodesAnim;
};

