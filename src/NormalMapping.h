#pragma once
#include <iostream>

#include "Texture2.h"
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"
#include "Primitive.h"

class NormalMapping
{
public:
	enum class TypeOfPrimitive : uint32_t
	{
		QUAD = 0,
		TRIANGLE = 1,
		CUBE = 2,
		PYRAMID = 3
	};
public:
	NormalMapping() = default;
	NormalMapping(const std::filesystem::path& pVertShader,
				  const std::filesystem::path& pFragShader,
			      const std::filesystem::path& pDiffuseTexturePath,
				  const std::filesystem::path& pNormalTexturePath,
				  TypeOfPrimitive pTypeOfPrimitive);


	void init(const std::filesystem::path& pVertShader,
			  const std::filesystem::path& pFragShader,
			  const std::filesystem::path& pDiffuseTexturePath,
			  const std::filesystem::path& pNormalTexturePath, 
			  TypeOfPrimitive pTypeOfPrimitive);

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
	std::shared_ptr<Primitive> mPrimitive;
	Mesh mMesh;

	Texture2 mDiffuseTexture;
	Texture2 mNormalTexture;
};