#pragma once
#include <array>
#include <filesystem>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Texture2.h"
#include "Mesh.h"
#include "Primitive.h"
#include "Shader.h"

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

	void setMVP(const glm::mat4& pModel, const glm::mat4& pProj, const glm::mat4& pView);
	glm::mat4& getMVP() noexcept;

	void render(Shader& pShader);

private:
	Mesh mSkybox;
	glm::mat4 mMVP;
};

