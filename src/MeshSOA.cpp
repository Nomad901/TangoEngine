#include "MeshSOA.h"

void MeshSOA::loadMesh(const std::filesystem::path& pPath)
{
}

void MeshSOA::destroy()
{
	mVAO.destroy();

	BasicMesh::mIndices.clear();
	BasicMesh::mMeshes.clear();
}

uint32_t MeshSOA::getNumberOfVertices() const noexcept
{
	return mVertexSOA.getNumberOfVertices();
}

BasicMesh::vertexFormat MeshSOA::getVertexFormat() const noexcept
{
	return BasicMesh::vertexFormat::vertexSOA;
}

void MeshSOA::render()
{
}
