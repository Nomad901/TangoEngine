
#pragma once
#include <iostream>
#include <format>
#include <vector>
#include <memory>

#include "glm.hpp"

#include "Transform.h"
#include "GBuffer.h"
#include "Shader.h"
#include "Model.h"

class DeferredLightSystem
{
public:
	enum class TypeShader : uint32_t
	{
		NULL_SHADER = 0,
		LIGHT_SHADER = 1,
		FRAME_SHADER = 2,
		NUMBER_SHADERS = 3
	};
public:
	DeferredLightSystem() = default;
	DeferredLightSystem(uint32_t pWidthWindow, uint32_t pHeightWindow,
		const std::vector<std::pair<glm::vec3, glm::vec3>> pLightPositionsAndColors);

	void init(uint32_t pWidthWindow, uint32_t pHeightWindow,
		const std::vector<std::pair<glm::vec3, glm::vec3>> pLightPositionsAndColors);

	void startGeometryPass();
	//
	// here u need to pass geometry 
	//
	void stopGeometryPass();
	void startLightPass(Camera pCamera, const glm::mat4& pProjMatrix);
	//
	// here u need to pass other geometry, like cubemaps, characters and so on;
	//
	void stopLightPassAndRenderFrame();

private:
	void geometryPassBeginning();
	void geometryPassEnding();

	void stencilPass(size_t pIndex, Camera pCamera, const glm::mat4& pProjMatrix);
	void lightPass(size_t pIndex, Camera pCamera, const glm::mat4& pProjMatrix);
	void renderQuad();

	void framePass(Camera pCamera, const glm::mat4& pProjMatrix);
	void finalPass();

	uint32_t getIndex(TypeShader pTypeShader);

private:
	uint32_t mWidthWindow{ 1280 }, mHeightWindow{ 720 };

	uint32_t mQuadVAO{}, mQuadVBO{};

	GBuffer mGBuffer;
	std::unique_ptr<Model> mSphereModel;

	std::array<std::unique_ptr<Shader>, static_cast<uint32_t>(TypeShader::NUMBER_SHADERS)> mShaders;

	std::vector<glm::vec3> mLightPositions;
	std::vector<glm::vec3> mLightColors;
};
