#pragma once
#include "BasicMesh.h"

class MeshAOS : public BasicMesh
{
public:
	MeshAOS() = default;

	void loadMesh(const std::filesystem::path& pPath) override;

private:

};

