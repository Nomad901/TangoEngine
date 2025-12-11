#include "MeshSOA.h"

void MeshSOA::loadMesh(const std::filesystem::path& pPath)
{
}

void MeshSOA::destroy()
{
	mVertexType.clearAllData();
	mVAO.destroy();
	mVBO.destroy();
	mEBO.destroy();
	mVBOLayout.destroy();

	BasicMesh::mIndices.clear();
	BasicMesh::mMeshes.clear();
}

uint32_t MeshSOA::getNumberOfVertices() const noexcept
{
	return mVertexType.getNumberOfVertices();
}

BasicMesh::vertexFormat MeshSOA::getVertexFormat() const noexcept
{
	return BasicMesh::vertexFormat::vertexSOA;
}

void MeshSOA::render()
{
}
