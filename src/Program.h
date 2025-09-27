#pragma once
#include "Initializer.h"

class Program
{
public:
	//Program(uint32_t pWindowWidth = 1920, uint32_t pWindowHeight = 1080);
	Program(uint32_t pWindowWidth = 1280, uint32_t pWindowHeight = 720);
	~Program();

	void run();

private:
	void input();
	void preDraw();
	void draw();

	void controlScreen();
	void controlCamera();
	void controlModel();
	void controlLight();

	void takerCursor();

	void setLights();
	void setLightCube();
	void setMaterials();
	void setModels();
	void setSkybox();

	void drawModels();
	void drawNormals();

	static void debugOutput(GLenum source,
							GLenum type,
							GLuint id,
							GLenum severity,
							GLsizei length,
							const GLchar* message,
							const void* userParam);
private:
	struct programProtperties
	{
		bool mProgIsRunning{ true };
		bool mWhiteScreen{ false };
		bool mTakeCursor{ true };
		bool mWireFrameMode{ false };

		uint32_t mWindowWidth, mWindowHeight;
		std::unordered_map<SDL_Keycode, bool> mKeyCodes;
		std::string mResourcePath;

		SDL_Window* mWindow;
		SDL_GLContext mContext;
		SDL_Event mEvent;

		Camera mCamera;

		Shader mShader;
		Shader mShaderSingleColor;
		Shader mShaderSecondScreen;
		Shader mSkyboxShader;

		UBO mUBO;

		FBO mFBO;
		std::vector<FBO> mFBOstrg;
		
		std::unique_ptr<Skybox> mSkybox;

		MousePicker mMousePicker;
		std::unique_ptr<Crosshair> mCrosshair;
	} mProgramProperties;

	struct lightProperties
	{
		glm::vec3 mPosLight{ 1.0f };
		LightManager mLightManager;
	} mLightProperties;

	struct materialProperties
	{
		glm::vec3 mAmbient{ 0.172f, 0.172f, 0.172f };
		glm::vec3 mDiffuse{ 0.160f, 0.160f, 0.160f };
		glm::vec3 mSpecular{ 1.0f };
		float mShines{ 32.0f };

		std::unique_ptr<Material> mMaterial;
	} mMaterialProperties;

	struct modelProperties
	{
		bool mRotatedMode{ false };
		float mDistanceFromCamera{ 5.0f };
		float mRotateDegree{ 1.0f };
		
		glm::mat4 mProjMatrix{ 1.0f };

		glm::vec3 mRotateForFloor{ 0.0f, 1.0f, 0.0f };
		glm::vec3 mLastPos{ 1.0f };

		std::unordered_map<std::string, std::shared_ptr<Primitive>> mPrimitives;
		std::vector<std::pair<Texture2, Texture2>> mTextures;
		std::vector<std::unique_ptr<Model>> mModel;
		Collider mCollider;
		FactoryMesh mFactoryMeshes;
		OBJLoader mOBJLoader;
		AssimpLoader mAssimpLoader;
	} mModelProperties;

	float surroundNum{ -1 };
	float insideNum{ 9 };
	std::vector<glm::mat4> mModels;
	uint32_t mUBOnumber;

	float mRadius{ 5.0f };

	UI mUI;
	friend class UI;
	friend class Initializer;
};