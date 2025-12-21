#pragma once
#include "BasicMesh.h"
#include "VertexAOS.h"

class MeshAOS : public BasicMesh
{
private:
	enum class indexType : uint32_t
	{
		POSITION_INDEX = 0,
		SCALE_INDEX = 1,
		ROTATION_INDEX = 2
	};
public:
	MeshAOS() = default;

	void loadMesh(const std::filesystem::path& pPath) override;
	void destroy() override;

	uint32_t getNumberOfVertices() const noexcept override;
	vertexFormat getVertexFormat() const noexcept override;

	void render() override;

private:
	void loadMeshFromScene(const aiScene* pScene, const std::filesystem::path& pPath);

	void populateBuffers();

	std::pair<uint32_t, uint32_t> getNumVerticesAndIndices(const aiScene* pScene);

	void initAllMeshes(const aiScene* pScene);
	void initSingleMesh(uint32_t pIndex, const aiMesh* pMesh);

	void initMaterials(const aiScene* pScene, const std::filesystem::path& pPath);

	void loadTexture(const std::filesystem::path& pPath, const aiMaterial* pMaterial, uint32_t pIndex);
	void loadDiffuseTexture(const std::filesystem::path& pPath, const aiMaterial* pMaterial, uint32_t pIndex);
	void loadSpecularTexture(const std::filesystem::path& pPath, const aiMaterial* pMaterial, uint32_t pIndex);
	void loadColors(const aiMaterial* pMaterial, uint32_t pIndex);
	void readNodeHierachy(float pAnimTime, const aiNode* pNode, const glm::mat4& pTransformation);
	const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, std::string_view pNodeName);

	void calcInterpolatedScale(aiVector3D& pScaling, float pAnimTimeTicks, const aiNodeAnim* pAiNodeAnim);
	void calcInterpolatedRotation(aiQuaternion& pRotation, float pAnimTimeTicks, const aiNodeAnim* pAiNodeAnim);
	void calcInterpolatedPosition(aiVector3D& pPosition, float pAnimTimeTicks, const aiNodeAnim* pAiNodeAnim);
	uint32_t findInterpolatedIndex(indexType pIndexType, float pAnimTicks, const aiNodeAnim* pAiNodeAnim);
	
private:
	std::vector<VertexAOS> mVertices;

	VAO mVAO;
	VBO mVBO;
	EBO	mEBO;
};

