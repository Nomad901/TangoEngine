#pragma once
#include <unordered_map>
#include <string>

#include "Mesh.h"

class FactoryMesh
{
public:
	FactoryMesh() = default;
	FactoryMesh(std::string_view pName, std::unique_ptr<Mesh> pMesh);

	void pushMesh(std::string_view pName, std::unique_ptr<Mesh> pMesh);
	void popMesh(std::string_view pName);
	size_t getSize() const noexcept;

	Mesh& getMesh(std::string_view pName);
	
	void render();

private:
	std::unordered_map<std::string, std::unique_ptr<Mesh>> mMeshes;
};

