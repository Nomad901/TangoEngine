#pragma once
#include <array>
#include <filesystem>
#include <memory>
#include <unordered_map>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Texture2.h"
#include "Mesh.h"
#include "Primitive.h"
#include "Shader.h"


class Skybox
{
public:
	enum class typeSkybox
	{
		CUBE = 0,
		SPHERE = 1
	};
	enum class SkyboxArtType : uint32_t
	{
		SPACE = 0,
		CLOUDS = 1,
		NUM_ARTS = 2
	};
	enum class PhotosExtensions
	{
		PNG = 0,
		JPG = 1
	};
public:
	Skybox() = default;
	Skybox(typeSkybox pTypeSkybox, const std::array<std::filesystem::path, 6>& pPaths, std::string_view pNameForSkybox, uint32_t pSlot);
	//
	// slot - slot for textures;
	// pPhotosExtension - can be png, jpg and so on. Was made in order to optimize the process of initialization skyboxes;
	//
	Skybox(typeSkybox pTypeSkybox, SkyboxArtType pSkyboxArtType, uint32_t pSlot);

	void init(typeSkybox pTypeSkybox, const std::array<std::filesystem::path, 6>& pPaths, std::string_view pNameForSkybox, uint32_t pSlot);
	void init(typeSkybox pTypeSkybox, SkyboxArtType pSkyboxArtType, uint32_t pSlot);

	Mesh& getMesh() noexcept;
	Texture2& getTexture() noexcept;
	
	void setMVP(const glm::mat4& pModel, const glm::mat4& pProj, const glm::mat4& pView);
	glm::mat4& getMVP() noexcept;

	void render(Shader& pShader);

private:
	auto initArtsOfSkybox() -> void;

	//
	// if u wanna add more arts into your enum class, then u need to update these functions after.
	// just add paths to getNamesOfArts() and names for skyboxes in initNamesForSkyboxes();
	//
	// ------------------------------------------------
	auto initNamesForSkyboxes() -> void;
	auto getNamesOfArts() -> std::vector<std::string>;
	// ------------------------------------------------

	auto getPhotosExtension(PhotosExtensions pPhotosExtension) -> std::string;
	auto addPathsToSkyboxArts(std::string_view pNameForSkybox, const std::array<std::filesystem::path, 6>& pPaths) -> void;

private:
	Mesh mSkybox;
	glm::mat4 mMVP;
	std::unordered_map<std::string, std::array<std::filesystem::path, 6>> mSkyboxArts;
	std::vector<std::string> mNamesForSkyboxes;
};

