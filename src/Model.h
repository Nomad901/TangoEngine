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
	Model(const glm::vec3& pOriginPos, 
		  const std::pair<Texture2, Texture2>& pTextures,
				std::pair<uint32_t, uint32_t> pSlots,
			    std::vector<std::unique_ptr<Mesh>>& pMeshes);
	Model(const glm::vec3& pOriginPos, const std::filesystem::path& pPath,
		  const std::pair<Texture2, Texture2>& pTextures,
				std::pair<uint32_t, uint32_t> pSlots);
	~Model() = default;
	
	//
	// init with the storage of meshes;
	// Textures - pair of meshes, like diffuse and specular
	//
	void init(const glm::vec3& pOriginPos,
			  const std::pair<Texture2, Texture2>& pTextures, 
					std::pair<uint32_t, uint32_t> pSlots,
					std::vector<std::unique_ptr<Mesh>>& pMeshes);
	//
	// init with .obj file
	// Textures - pair of meshes, like diffuse and specular
	//
	void init(const glm::vec3& pOriginPos, const std::filesystem::path& pPath,
			  const std::pair<Texture2, Texture2>& pTextures,
				    std::pair<uint32_t, uint32_t> pSlots);

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

	std::pair<Texture2, Texture2>& getTextures() noexcept;
	std::pair<uint32_t, uint32_t> getSlots() const noexcept;

	void takeModel(bool pTake);
	bool modelIsTaken() const noexcept;

	void render(Shader& pShader);
	void render();

private:
	void updateUniforms(Shader& pShader);

private:
	bool mIsTaken;

	Material* mMaterial;
	std::pair<Texture2, Texture2> mTextures;
	std::pair<uint32_t, uint32_t> mSlots;
	std::vector<std::unique_ptr<Mesh>> mMeshes;

	OBJLoader mOBJLoader;

	glm::vec3 mOriginPos;
	glm::vec3 mPos;
	glm::vec3 mSize;
};

