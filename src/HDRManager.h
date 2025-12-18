#pragma once
#include <iostream>
#include <vector>

#include "glm.hpp"

#include "Texture2.h"
#include "Shader.h"
#include "Model.h"

class HDRManager
{
public:
	HDRManager() = default;
	HDRManager(uint32_t pScreenWidth, uint32_t pScreenHeight,
			   const std::vector<glm::vec3>& pLightPos,
			   const std::vector<glm::vec3>& pLightColors);

	void init(uint32_t pScreenWidth, uint32_t pScreenHeight,
			  const std::vector<glm::vec3>& pLightPos,
			  const std::vector<glm::vec3>& pLightColors);
	void initShaders(const std::filesystem::path& pModelVertPath,
					 const std::filesystem::path& pModelFragPath,
					 const std::filesystem::path& pHDRVertPath,
					 const std::filesystem::path& pHDRFragPath);

	void startHDRPass();
	void stopHDRPass();
	void renderHDR(bool pTurnOnHDR = true, float pExposure = 1.0f);

	void setLightPositions(const std::vector<glm::vec3>& pLightPositions);
	void setLightColors(const std::vector<glm::vec3>& pLightColors);
	auto getLightPositions() const noexcept -> const std::vector<glm::vec3>&;
	auto getLightColors() const noexcept -> const std::vector<glm::vec3>&;

	Shader& getModelShader() noexcept;
	Shader& getHDRShader() noexcept;

	bool shadersAreInitialized() const noexcept;

private:
	void bindAll();

	void initFBO();
	void createQuad();
	void renderQuad();

private:
	uint32_t mQuadVAO{};
	uint32_t mFBOid{};
	uint32_t mColorBuffer{};

	uint32_t mScreenWidth{}, mScreenHeight{};

	Shader mModelShader, mHDRShader;
	
	std::vector<glm::vec3> mLightPositions;
	std::vector<glm::vec3> mLightColors;

	bool mShadersAreInitialized{ false };
};

