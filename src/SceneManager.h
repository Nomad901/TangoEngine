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
#include "Shader.h"
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

class Renderer;

class SceneManager
{
public:
	void setAll();
	void setLights();
	void setLightCube();
	void setMaterials();
	void setModels();
	void setSkybox();

public:
	struct programProperties
	{
		bool mProgIsRunning{ true };
		bool mWhiteScreen{ false };
		bool mTakeCursor{ true };
		bool mWireFrameMode{ false };
		
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


		int32_t currentTexture{ 0 };
	};

	struct lightProperties
	{
		glm::vec3 mPosLight{ 1.0f };
		glm::vec3 mLightDir{ 1.0f };
		float mSoftness{ 7.0f };
		float mRadius{ 50.0f };

		std::vector<glm::vec3> lightPositions;
		std::vector<glm::vec3> lightColors;

		LightManager mLightManager;
	};

	struct materialProperties
	{
		glm::vec3 mAmbient{ 0.172f, 0.172f, 0.172f };
		glm::vec3 mDiffuse{ 0.160f, 0.160f, 0.160f };
		glm::vec3 mSpecular{ 1.0f };
		float mShines{ 32.0f };

		std::unique_ptr<Material> mMaterial;
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
	};

public:
	programProperties& getProgramProperties() noexcept;
	lightProperties& getLightProperties() noexcept;
	materialProperties& getMaterialProperties() noexcept;
	modelProperties& getModelProperties() noexcept;

private:
	programProperties mProgramProperties;
	lightProperties mLightProperties;
	materialProperties mMaterialProperties;
	modelProperties mModelProperties;

	friend class UI;
	friend class Renderer;
};

