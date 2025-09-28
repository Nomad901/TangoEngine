#pragma once
#include "Includes.h"

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

		Camera mCamera;
		glm::mat4 mViewMatrix;

		MousePicker mMousePicker;
		ShaderManager mShaders;
		UI mUI;
		UBO mUBO;
		std::unique_ptr<Skybox> mSkybox;
		std::unique_ptr<Crosshair> mCrosshair;

		FBO mFBO;
		std::vector<FBO> mFBOstrg;
	};

	struct lightProperties
	{
		glm::vec3 mPosLight{ 1.0f };
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

		PrimitivesManager mPrimitivesManager;
		TextureManager mTextureManager;
		ModelManager mModelManager;
		Collider mCollider;
		FactoryMesh mFactoryMeshes;
		OBJLoader mOBJLoader;
		AssimpLoader mAssimpLoader;
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

	glm::vec3 posMuseum{ 1.0f };
	glm::vec3 sizeMuseum{ 1.0f };

	friend class UI;
	friend class Renderer;
};

