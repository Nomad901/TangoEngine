#pragma once
#include <filesystem>

#include "Mesh.h"
#include "Texture2.h"
#include "Shader.h"
#include "Material.h"
#include "OBJLoader.h"

class Model
{
public:
	Model() = default;
	Model(const glm::vec3& pOriginPos, Material* pMaterialPtr,
		  const std::vector<Texture2>& pTextures, 
		  std::vector<std::unique_ptr<Mesh>>& pMeshes);
	Model(const glm::vec3& pOriginPos, Material* pMaterialPtr, 
		  const std::filesystem::path& pPath, const std::vector<Texture2>& pTextures);
	~Model() = default;
	
	//
	// init with the storage of meshes
	//
	void init(const glm::vec3& pOriginPos, Material* pMaterialPtr,
			  const std::vector<Texture2>& pTextures,
			  std::vector<std::unique_ptr<Mesh>>& pMeshes);
	//
	// init with .obj file
	//
	void init(const glm::vec3& pOriginPos, Material* pMaterialPtr, 
			  const std::filesystem::path& pPath, const std::vector<Texture2>& pTextures);

	void initMVP(const glm::mat4& pProjMatrix, const glm::mat4& pViewMatrix, const glm::vec3& pTranslation, 
				 const std::pair<float, glm::vec3>& pDegreeRotate, const glm::vec3& pScale);
	glm::mat4 getModelMatrix() const noexcept;
	glm::mat4 getViewMatrix() const noexcept;
	glm::mat4 getProjMatrix() const noexcept;
	glm::mat4 getMVP();

	void setUniforms(Shader& pShader, const glm::vec4& pColor);

	void setPos(const glm::vec3& pPos);
	void setSize(const glm::vec3& pSize);
	glm::vec3 getPos() const noexcept;
	glm::vec3 getSize() const noexcept;
	glm::vec3 getOriginPos() const noexcept;

	void takeModel(bool pTake);
	bool modelIsTaken() const noexcept;

	void render(Shader& pShader);
	void render();

private:
	void updateUniforms(Shader& pShader);

private:
	bool mIsTaken;

	Material* mMaterial;
	std::vector<Texture2> mTextures;
	std::vector<std::unique_ptr<Mesh>> mMeshes;

	OBJLoader mOBJLoader;

	glm::vec3 mOriginPos;
	glm::vec3 mPos;
	glm::vec3 mSize;
};

