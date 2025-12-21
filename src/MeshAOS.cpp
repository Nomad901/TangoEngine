#include "MeshAOS.h"

void MeshAOS::loadMesh(const std::filesystem::path& pPath)
{

}

void MeshAOS::destroy()
{

}

uint32_t MeshAOS::getNumberOfVertices() const noexcept
{
	return mVertices.size();
}

BasicMesh::vertexFormat MeshAOS::getVertexFormat() const noexcept
{
	return vertexFormat::vertexAOS;
}

void MeshAOS::render()
{

}

void MeshAOS::loadMeshFromScene(const aiScene* pScene, const std::filesystem::path& pPath)
{
}
