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
	enum class States : uint32_t
	{
		UNINITIALIZED = 0,
		INITIALIZED = 1,
		STARTED = 2,
		STOPED = 3
	};
public:
	HDRManager() = default;
	HDRManager(uint32_t pScreenWidth, uint32_t pScreenHeight);

	void init(uint32_t pScreenWidth, uint32_t pScreenHeight);
	void initShaders(const std::filesystem::path& pHDRVertPath,
		const std::filesystem::path& pHDRFragPath);

	void startHDRPass();
	void stopHDRPass();
	void renderHDR(bool pTurnOnHDR = true, float pExposure = 1.0f);

	Shader& getHDRShader() noexcept;

	States getCurrentState() const noexcept;

private:
	void bindAll(bool pTurnOnHDR, float pExposure);

	void initFBO();
	void createQuad();
	void renderQuad();

private:
	uint32_t mQuadVAO{};
	uint32_t mFBOid{};
	uint32_t mColorBuffer{};

	uint32_t mScreenWidth{}, mScreenHeight{};

	Shader mHDRShader;

	States mCurrentState{ States::UNINITIALIZED };
};

