#pragma once
#include <iostream>
#include <format>
#include <unordered_map>
#include <memory>
#include <array>
#include <random>

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "SDL3/SDL.h"
#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include "FactoryMesh.h"
#include "thirdPersonCam.h"
#include "Primitive.h"
#include "OBJLoader.h"
#include "UI.h"
#include "LightManager.h"
#include "Collider.h"
#include "Crosshair.h"
#include "FBO.h"
#include "Skybox.h"
#include "UBO.h"
#include "AssimpLoader.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "PrimitivesManager.h"
#include "ModelManager.h"
#include "FaultFormationTerrain.h"
#include "MidpointDispTerrain.h"
#include "TextureGenerator.h"
#include "FractalNoiseTerrain.h"
#include "GBuffer.h"
#include "FontSystem.h"
#include "FPSRegulator.h"
#include "Timer.h"
#include "AnimatorManager.h"
#include "DeferredLightSystem.h"
#include "WaterRenderer.h"
#include "WaterSSR.h"
#include "NormalMapping.h"

class Renderer;

class SceneManager
{
public:
	void setAll();
	void setLights();
	void setLightCube();
	void setModels();
	void setSkybox();

public:
	struct programProperties
	{
		bool mProgIsRunning{ true };
		bool mWhiteScreen{ false };
		bool mTakeCursor{ true };
		bool mWireFrameMode{ false };
		bool mIsIn3rdPersonCamera{ true };

		// for light-block, in order to making block far away and conversely
		float mRadius{ 5.0f };

		uint32_t mWindowWidth, mWindowHeight;
		std::string mResourcePath;
		SDL_Window* mWindow;
		SDL_GLContext mContext;

		thirdPersonCam mThirdPersonCam;
		Camera mSecondCamera;
		Camera mCamera;
		glm::mat4 mViewMatrix;
		bool mNoclip{ false };
		
		FontSystem mFontSystem;
		glm::vec2 mPosFont{ 0.0f, 0.0f };
		glm::vec3 mColorFont{ 1.0f, 1.0f, 1.0f };
		float mScaleFont{ 0.5f };

		ShaderManager mShaders;
		UI mUI;
		UBO mUBO;
		GBuffer mGBuffer;
		std::unique_ptr<Skybox> mSkybox;
		std::unique_ptr<Crosshair> mCrosshair;

		std::unique_ptr<FBO> mFBO;
		std::vector<FBO> mFBOstrg;
		
		Timer mTimer;
		FPSRegulator mFPSRegulator;

		WaterRenderer mWaterRenderer;
		std::unique_ptr<WaterFBO> mWaterFBO;
		std::vector<Water> mWaterTiles;
		glm::vec3 mWaterPos{ glm::vec3(0.0f, 10.0f, 0.0f) };
		glm::vec3 mWaterScale{ glm::vec3(122.0f, 0.0f, 122.0f) };
		glm::vec4 mWaterColor{ glm::vec4(0.0f, 0.3f, 0.5f, 1.0f) };

		bool mWindowWasChanged{ false };

		int32_t currentTexture{ 0 };

		const float NEAR_PLANE = 0.1f;
		const float FAR_PLANE  = 1000.0f;

		NormalMapping<Quad> mNormalMapping;

		GBuffer mWaterGBuffer;

		WaterSSR mWaterSSR;
	};

	struct lightProperties
	{
		glm::vec3 mPosLight{ 1.0f };
		glm::vec3 mLightDir{ 1.0f };
		float mSoftness{ 7.0f };
		float mRadius{ 50.0f };

		std::vector<glm::vec3> lightPositions;
		std::vector<glm::vec3> lightColors;

		DeferredLightSystem mDeferredLightSystem;

		LightManager mLightManager;

		DirectionalLight mSun;
	};

	struct modelProperties
	{
		bool mRotatedMode{ false };
		float mDistanceFromCamera{ 5.0f };
		float mRotateDegree{ 1.0f };

		glm::mat4 mProjMatrix{ 1.0f };

		glm::vec3 mRotateForFloor{ 0.0f, 1.0f, 0.0f };
		glm::vec3 mLastPos{ 1.0f };
		glm::vec3 mPlayerPos{ 1.0f };

		std::unique_ptr<Terrain> mTerrain;
		PrimitivesManager mPrimitivesManager;
		TextureManager mTextureManager;
		ModelManager mModelManager;
		FactoryMesh mFactoryMeshes;
		Collider mCollider;
		OBJLoader mOBJLoader;
		AssimpLoader mAssimpLoader;

		TextureGenerator mTextureGenerator;
		std::unique_ptr<Texture2> mTexture;

		std::unique_ptr<SkinnedMesh> mWaterTerrain;

		AnimatorManager mAnimatorManager;

		int32_t displayedQuad{ 0 };

		glm::vec3 mCharPos{ glm::vec3(114.0f, 65.0f, 409.0f) };
		glm::vec3 mCharRotation{ glm::vec3(-90.0f, 0.0f, 0.0f) };
		glm::vec3 mCharScale{ glm::vec3(1.0f, 1.0f, 1.0f) };

		glm::vec4 mPlaneTerrainHeight = glm::vec4(0.0f, -1.0f, 0.0f, 55.0f);
	};

public:
	programProperties& getProgramProperties() noexcept;
	lightProperties& getLightProperties() noexcept;
	modelProperties& getModelProperties() noexcept;

private:
	programProperties mProgramProperties;
	lightProperties mLightProperties;
	modelProperties mModelProperties;

	friend class UI;
	friend class Renderer;
};

