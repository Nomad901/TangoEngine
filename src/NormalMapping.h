#pragma once
#include <iostream>

#include "Texture2.h"
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"
#include "Primitive.h"

template<typename T>
concept primitiveClass = std::is_base_of_v<Primitive, T>;

template<typename T>
requires primitiveClass<T>
class NormalMapping
{
public:
	NormalMapping() = default;
	NormalMapping(const std::filesystem::path& pVertShader,
				  const std::filesystem::path& pFragShader,
			      const std::filesystem::path& pDiffuseTexturePath,
				  const std::filesystem::path& pNormalTexturePath);

	void init(const std::filesystem::path& pVertShader,
			  const std::filesystem::path& pFragShader,
			  const std::filesystem::path& pDiffuseTexturePath,
			  const std::filesystem::path& pNormalTexturePath);

	Shader& getShader() noexcept;
	Mesh& getMesh() noexcept;
	Texture2& getDiffuseTexture() noexcept;
	Texture2& getNormalTexture() noexcept;
	
	void render(Transform& pTransform,
				const glm::mat4& pViewMatrix,
				const glm::mat4& pProjMatrix,
				const glm::vec3& pPosLight,
				const glm::vec3& pPosCamera);

private:
	void bindAll(Transform& pTransform,
				 const glm::mat4& pViewMatrix,
				 const glm::mat4& pProjMatrix,
				 const glm::vec3& pPosLight,
				 const glm::vec3& pPosCamera);
	
private:
	Shader mNormalMappingShader;
	Mesh mMesh;

	Texture2 mDiffuseTexture;
	Texture2 mNormalTexture;
};

