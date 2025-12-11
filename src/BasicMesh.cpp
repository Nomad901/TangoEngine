#include "BasicMesh.h"

uint32_t BasicMesh::getNumberOfMeshes() const noexcept
{
	return mMeshes.size();
}

uint32_t BasicMesh::getNumberOfIndices() const noexcept
{
	return mIndices.size();
}

const Transform& BasicMesh::getTransform() const noexcept
{
	return mTransform;
}

Transform& BasicMesh::getTransform() noexcept
{
	return mTransform;
}

void BasicMesh::setTransform(const Transform& pTransform)
{
	mTransform = pTransform;
}

const PBRMaterial& BasicMesh::getMaterial() const noexcept
{
	return mMaterial;
}

PBRMaterial& BasicMesh::getMaterial() noexcept
{
	return mMaterial;
}
