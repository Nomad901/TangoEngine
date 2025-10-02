#pragma once
#include <filesystem>

#include "Mesh.h"
#include "Texture2.h"
#include "Shader.h"
#include "Material.h"
#include "OBJLoader.h"
#include "AssimpLoader.h"

class Model
{
public:
	Model() = default;
	Model(const glm::vec3& pOriginPoss, std::vector<std::unique_ptr<Mesh>>& pMeshes);
	Model(const glm::vec3& pOriginPos, const std::filesystem::path& pPath);
	~Model() = default;
	
	//
	// init with the storage of meshes;
	// Textures - pair of meshes, like diffuse and specular
	//
	void init(const glm::vec3& pOriginPos, std::vector<std::unique_ptr<Mesh>>& pMeshes);
	//
	// init with any file
	// Textures - pair of meshes, like diffuse and specular
	//
	void init(const glm::vec3& pOriginPos, const std::filesystem::path& pPath);
	//
	// init only .obj files
	// Assimp is too slow, but my obj loader is much faster.
	// if u can load models in obj - load them, this will be much faster;
	//
	void initOBJmodel(const glm::vec3& pOriginPos, const std::filesystem::path& pPath);

	void setInstancedData(const std::vector<glm::mat4>& pMatrices, GLenum pUsage);

	void initMVP(const glm::mat4& pProjMatrix, const glm::mat4& pViewMatrix, const glm::vec3& pTranslation, 
				 const std::pair<float, glm::vec3>& pDegreeRotate, const glm::vec3& pScale);
	void initMVP(const glm::mat4& pProjMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pModel);

	glm::mat4 getModelMatrix() const noexcept;
	glm::mat4 getViewMatrix() const noexcept;
	glm::mat4 getProjMatrix() const noexcept;
	glm::mat4 getMVP();

	std::vector<std::unique_ptr<Mesh>>& getMeshes() noexcept;

	void setUniforms(Shader& pShader, const glm::vec4& pColor);

	void setPos(const glm::vec3& pPos);
	void setSize(const glm::vec3& pSize);
	glm::vec3 getPos() const noexcept;
	glm::vec3 getSize() const noexcept;
	glm::vec3 getOriginPos() const noexcept;

	void takeModel(bool pTake);
	bool modelIsTaken() const noexcept;

	void render(Shader& pShader, const glm::vec3& pColor = glm::vec3(1.0f, 1.0f, 1.0f));
	void renderInstanced(uint32_t pNumber);

private:
	void updateUniforms(Shader& pShader, const glm::vec3& pColor);

private:
	bool mIsTaken;

	Material* mMaterial;
	std::vector<std::unique_ptr<Texture2>> mTextures;
	std::pair<uint32_t, uint32_t> mSlots;
	std::vector<std::unique_ptr<Mesh>> mMeshes;

	OBJLoader mOBJLoader;
	AssimpLoader mAssimpLoader;

	glm::vec3 mOriginPos;
	glm::vec3 mPos;
	glm::vec3 mSize;
};

