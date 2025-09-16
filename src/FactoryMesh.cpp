#include "FactoryMesh.h"

FactoryMesh::FactoryMesh(std::string_view pName, std::unique_ptr<Mesh> pMesh)
{
	pushMesh(pName, std::move(pMesh));
}

void FactoryMesh::pushMesh(std::string_view pName, std::unique_ptr<Mesh> pMesh)
{
	mMeshes.insert_or_assign(std::string(pName), std::move(pMesh));
}

void FactoryMesh::popMesh(std::string_view pName)
{
	if (mMeshes.contains(std::string(pName)))
	{
		mMeshes.erase(std::string(pName));
	}
	else 
		std::cout << "The storage doesnt contain the mesh!\n";
}

size_t FactoryMesh::getSize() const noexcept
{
	return mMeshes.size();
}

Mesh& FactoryMesh::getMesh(std::string_view pName)
{
	if (!mMeshes.contains(std::string(pName)))
	{
		Mesh errorMesh;
		std::cout << "The storage doesnt contain the mesh!\n";
		return errorMesh;
	}
	return *mMeshes[std::string(pName)].get();
}

void FactoryMesh::render()
{
	for (auto& [key, value] : mMeshes)
	{
		value->draw();
	}
}
