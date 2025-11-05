#include "Skybox.h"

Skybox::Skybox(typeSkybox pTypeSkybox, const std::array<std::filesystem::path, 6>& pPaths, std::string_view pNameForSkybox, uint32_t pSlot)
{
	init(pTypeSkybox, pPaths, pNameForSkybox, pSlot);
}

Skybox::Skybox(typeSkybox pTypeSkybox, SkyboxArtType pSkyboxArtType, uint32_t pSlot)
{
	initArtsOfSkybox();
	init(pTypeSkybox, pSkyboxArtType, pSlot);
}

void Skybox::init(typeSkybox pTypeSkybox, const std::array<std::filesystem::path, 6>& pPaths, std::string_view pNameForSkybox, uint32_t pSlot)
{
	Texture2 tmpTexture;
	tmpTexture.initCubeMaps(pPaths);
	addPathsToSkyboxArts(pNameForSkybox, pPaths);
	if (pTypeSkybox == typeSkybox::CUBE)
	{
		std::shared_ptr<Primitive> cube = std::make_shared<Cube>(tmpTexture, pSlot, true);
		std::weak_ptr<Primitive> weakCube = cube;
		mSkybox.init(weakCube);
	}
	else if (pTypeSkybox == typeSkybox::SPHERE)
	{
		std::shared_ptr<Primitive> sphere = std::make_shared<Sphere>(tmpTexture, pSlot, 3000.0f, 32, 16);
		std::weak_ptr<Primitive> weakSphere = sphere;
		mSkybox.init(weakSphere);
	}
	getTexture().setTarget(GL_TEXTURE_CUBE_MAP);
}

void Skybox::init(typeSkybox pTypeSkybox, SkyboxArtType pSkyboxArtType, uint32_t pSlot)
{
	Texture2 tmpTexture;
	tmpTexture.initCubeMaps(mSkyboxArts[mNamesForSkyboxes[static_cast<uint32_t>(pSkyboxArtType)]]);
	if (pTypeSkybox == typeSkybox::CUBE)
	{
		std::shared_ptr<Primitive> cube = std::make_shared<Cube>(tmpTexture, pSlot, true);
		std::weak_ptr<Primitive> weakCube = cube;
		mSkybox.init(weakCube);
	}
	else if (pTypeSkybox == typeSkybox::SPHERE)
	{
		std::shared_ptr<Primitive> sphere = std::make_shared<Sphere>(tmpTexture, pSlot, 3000.0f, 32, 16);
		std::weak_ptr<Primitive> weakSphere = sphere;
		mSkybox.init(weakSphere);
	}
	getTexture().setTarget(GL_TEXTURE_CUBE_MAP);
}

Mesh& Skybox::getMesh() noexcept
{
	return mSkybox;
}

Texture2& Skybox::getTexture() noexcept
{
	return mSkybox.getPrimitive().getSingleTex();
}

void Skybox::setMVP(const glm::mat4& pModel, const glm::mat4& pProj, const glm::mat4& pView)
{
	mMVP = pProj * pView * pModel;
}

glm::mat4& Skybox::getMVP() noexcept
{
	return mMVP;
}

void Skybox::render(Shader& pShader)
{
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);

	pShader.bind();
	pShader.setMatrixUniform4fv("uMVP", mMVP);
	
	getTexture().bind();
	mSkybox.draw();
	
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
}

void Skybox::initArtsOfSkybox()
{
	initNamesForSkyboxes();
	std::vector<std::string> names = getNamesOfArts();

	mSkyboxArts.reserve(names.size());
	for (size_t i = 0; i < names.size(); ++i)
	{
		if (std::filesystem::exists(names[i] + "_lf.png"))
		{
			std::array<std::filesystem::path, 6> skyboxTextures;
			
			skyboxTextures[0] = names[i] + "_lf.png"; 
			skyboxTextures[1] = names[i] + "_rt.png"; 
			skyboxTextures[2] = names[i] + "_up.png"; 
			skyboxTextures[3] = names[i] + "_dn.png"; 
			skyboxTextures[4] = names[i] + "_ft.png"; 
			skyboxTextures[5] = names[i] + "_bk.png";
			
			mSkyboxArts[mNamesForSkyboxes[static_cast<uint32_t>(SkyboxArtType::SPACE) + i]] = skyboxTextures;
		}
	}
}

auto Skybox::initNamesForSkyboxes() -> void
{
	mNamesForSkyboxes.resize(static_cast<uint32_t>(SkyboxArtType::NUM_ARTS));
	mNamesForSkyboxes[static_cast<uint32_t>(SkyboxArtType::SPACE)] = "Space";
	mNamesForSkyboxes[static_cast<uint32_t>(SkyboxArtType::CLOUDS)] = "Clouds";
	
	mSkyboxArts[mNamesForSkyboxes[static_cast<uint32_t>(SkyboxArtType::SPACE)]]  = std::array<std::filesystem::path, 6>();
	mSkyboxArts[mNamesForSkyboxes[static_cast<uint32_t>(SkyboxArtType::CLOUDS)]] = std::array<std::filesystem::path, 6>();
}

auto Skybox::getNamesOfArts() -> std::vector<std::string>
{
	std::string resourcesPath = RESOURCES_PATH;
	std::vector<std::string> names = {
		resourcesPath + "Skybox/ulukai/corona",
		resourcesPath + "Skybox/cloudy/bluecloud"
	};
	return names;
}

auto Skybox::getPhotosExtension(PhotosExtensions pPhotosExtension) -> std::string
{
	std::string photosExtension;
	switch (pPhotosExtension)
	{
	case Skybox::PhotosExtensions::PNG:
		photosExtension = ".png";
		break;
	case Skybox::PhotosExtensions::JPG:
		photosExtension = ".jpg";
		break;
	default:
		std::cout << "You were not supposed to reach this point! ang99 easter egg\n";
		exit(1);
		break;
	}
	return photosExtension;
}

auto Skybox::addPathsToSkyboxArts(std::string_view pNameForSkybox, const std::array<std::filesystem::path, 6>& pPaths) -> void
{
	for (auto& i : pPaths)
	{
		if (!std::filesystem::exists(i))
			return;
	}
	mSkyboxArts[std::string(pNameForSkybox)] = pPaths;
}