#include "Skybox.h"

Skybox::Skybox(typeSkybox pTypeSkybox, const std::array<std::filesystem::path, 6>& pPaths, uint32_t pSloth)
{
	init(pTypeSkybox, pPaths, pSloth);
}

void Skybox::init(typeSkybox pTypeSkybox, const std::array<std::filesystem::path, 6>& pPaths, uint32_t pSloth)
{
	if (pTypeSkybox == typeSkybox::CUBE)
	{
		mTexture.initCubeMaps(pPaths);
		std::shared_ptr<Primitive> primitive = std::make_shared<Cube>(mTexture, pSloth, true);
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

Texture2& Skybox::getTextures() noexcept
{
	return mTexture;
}

void Skybox::render()
{
}
