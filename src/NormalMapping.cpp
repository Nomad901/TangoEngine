#include "NormalMapping.h"

NormalMapping::NormalMapping(const std::filesystem::path& pVertShader, 
							 const std::filesystem::path& pFragShader, 
							 typeOfMesh pTypeOfMesh,
							 const Transform& pTransform)
{
	init(pVertShader, pFragShader, pTypeOfMesh, pTransform);
}

void NormalMapping::init(const std::filesystem::path& pVertShader, 
						 const std::filesystem::path& pFragShader, 
						 typeOfMesh pTypeOfMesh,
						 const Transform& pTransform)
{
	switch (pTypeOfMesh)
	{
	case NormalMapping::typeOfMesh::QUAD:
		std::shared_ptr<Quad> quad = std::make_shared<Quad>();
		break;
	case NormalMapping::typeOfMesh::TRIANGLE:
		break;
	case NormalMapping::typeOfMesh::CUBE:
		break;
	case NormalMapping::typeOfMesh::PYRAMID:
		break;
	}
	
	mNormalMappingShader.init(pVertShader, pFragShader);
	mMeshTransform = pTransform;
}

Shader& NormalMapping::getShader() noexcept
{
	return mNormalMappingShader;
}

Mesh& NormalMapping::getMesh() noexcept
{
	return mMesh;
}

void NormalMapping::render()
{
}
