#include "Skybox.h"

Skybox::Skybox(typeSkybox pTypeSkybox, const std::array<std::filesystem::path, 6>& pPaths, uint32_t pSlot)
{
	init(pTypeSkybox, pPaths, pSlot);
}

void Skybox::init(typeSkybox pTypeSkybox, const std::array<std::filesystem::path, 6>& pPaths, uint32_t pSlot)
{
	if (pTypeSkybox == typeSkybox::CUBE)
	{
		mTexture.initCubeMaps(pPaths);
		std::shared_ptr<Primitive> primitive = std::make_shared<Cube>(mTexture, pSlot, true);
		std::weak_ptr<Primitive> weakCube = primitive;
		mSkybox.init(weakCube);
	}
	else if (pTypeSkybox == typeSkybox::SPHERE)
	{

	}
}

Mesh& Skybox::getMesh() noexcept
{
	return mSkybox;
}

Texture2& Skybox::getTexture() noexcept
{
	return mTexture;
}

void Skybox::render()
{
}
