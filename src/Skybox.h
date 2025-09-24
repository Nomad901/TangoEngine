#pragma once
#include <array>
#include <filesystem>
#include <memory>

#include "Texture2.h"
#include "Mesh.h"
#include "Primitive.h"

enum class typeSkybox
{
	CUBE = 0,
	SPHERE = 1
};

class Skybox
{
public:
	Skybox() = default;
	Skybox(typeSkybox pTypeSkybox, const std::array<std::filesystem::path, 6>& pPaths, uint32_t pSlot);
	
	void init(typeSkybox pTypeSkybox, const std::array<std::filesystem::path, 6>& pPaths, uint32_t pSlot);

	Mesh& getMesh() noexcept;
	Texture2& getTexture() noexcept;

	void render();

private:
	Mesh mSkybox;
};

