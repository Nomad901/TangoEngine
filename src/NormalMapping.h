#pragma once
#include <iostream>

#include "Texture2.h"
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"

class NormalMapping
{
public:
	enum class typeOfMesh : uint32_t
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
				  typeOfMesh pTypeOfMesh,
				  const Transform& pTransform);

	void init(const std::filesystem::path& pVertShader,
			  const std::filesystem::path& pFragShader,
			  typeOfMesh pTypeOfMesh,
			  const Transform& pTransform);

	Shader& getShader() noexcept;
	Mesh& getMesh() noexcept;

	void render();

private:
	void bindAll();
	
private:
	Transform mMeshTransform;
	Shader mNormalMappingShader;
	Mesh mMesh;

	Texture2 mDiffuseTexture;
	Texture2 mNormalTexture;
};

