#pragma once
#include <filesystem>

#include "Mesh.h"
#include "Texture2.h"
#include "Shader.h"
#include "Material.h"
#include "OBJLoader.h"
#include "AssimpLoader.h"

enum class typeModels
{
	OBJ = 0,
	ANY = 1
};

class Model
{
public:
	Model() = default;
	Model(const glm::vec3& pOriginPos, 
		  const std::pair<Texture2&, Texture2&>& pTextures,
				std::pair<uint32_t, uint32_t> pSlots,
			    std::vector<std::unique_ptr<Mesh>>& pMeshes);
	Model(const glm::vec3& pOriginPos, const std::filesystem::path& pPath, typeModels pType,
		  const std::pair<Texture2&, Texture2&>& pTextures,
				std::pair<uint32_t, uint32_t> pSlots);
	~Model() = default;
	
	//
	// init with the storage of meshes;
	// Textures - pair of meshes, like diffuse and specular
	//
	void init(const glm::vec3& pOriginPos,
			  const std::pair<Texture2&, Texture2&>& pTextures,
					std::pair<uint32_t, uint32_t> pSlots,
					std::vector<std::unique_ptr<Mesh>>& pMeshes);
	//
	// init with any file
	// Textures - pair of meshes, like diffuse and specular
	//
	void init(const glm::vec3& pOriginPos, const std::filesystem::path& pPath,
		      const std::pair<Texture2&, Texture2&>& pTextures,
				    std::pair<uint32_t, uint32_t> pSlots);
	//
	// init only .obj files
	// Assimp is too slow, but my obj loader is much faster.
	// if u can load models in obj - load them, this will be much faster;
	//
	void initOBJmodel(const glm::vec3& pOriginPos, const std::filesystem::path& pPath,
					  const std::pair<Texture2&, Texture2&>& pTextures,
					  std::pair<uint32_t, uint32_t> pSlots);

	void setInstancedData(const std::vector<glm::mat4>& pMatrices, GLenum pUsage);

	void initMVP(const glm::mat4& pProjMatrix, const glm::mat4& pViewMatrix, const glm::vec3& pTranslation, 
				 const std::pair<float, glm::vec3>& pDegreeRotate, const glm::vec3& pScale);
	void initMVP(const glm::mat4& pProjMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pModel);

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

	Texture2& getFirstTex() noexcept;
	Texture2& getSecondTex() noexcept;
 	std::pair<uint32_t, uint32_t> getSlots() const noexcept;

	void takeModel(bool pTake);
	bool modelIsTaken() const noexcept;

	void render(Shader& pShader);
	void render();
	void renderInstanced(uint32_t pNumber);

private:
	void updateUniforms(Shader& pShader);

private:
	bool mIsTaken;

	Material* mMaterial;
	std::pair<Texture2, Texture2> mTextures;
	std::pair<uint32_t, uint32_t> mSlots;
	std::vector<std::unique_ptr<Mesh>> mMeshes;

	OBJLoader mOBJLoader;
	AssimpLoader mAssimpLoader;

	glm::vec3 mOriginPos;
	glm::vec3 mPos;
	glm::vec3 mSize;
};

