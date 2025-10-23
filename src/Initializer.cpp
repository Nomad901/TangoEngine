#include "Initializer.h"
#include "Program.h"

Initializer::Initializer(SceneManager* pSceneManager)
{
	mSceneManager = pSceneManager;
}

void Initializer::init(bool pInitAll)
{
	// resource path
	mSceneManager->getProgramProperties().mResourcePath = RESOURCES_PATH;

	// camera
	mSceneManager->getProgramProperties().mCamera.setSensivity(0.2f);
	// projection matrix
	mSceneManager->getModelProperties().mProjMatrix = glm::perspective(glm::radians(45.0f), (float)mSceneManager->getProgramProperties().mWindowWidth /
		(float)mSceneManager->getProgramProperties().mWindowHeight, 0.1f, 2000.0f);
	// fbo
	mSceneManager->getProgramProperties().mFBO = std::make_unique<FBO>(mSceneManager->getProgramProperties().mWindowWidth, mSceneManager->getProgramProperties().mWindowHeight,
																	   glm::vec2(0.0f, 0.0f), glm::vec2(600.0f, 600.0f));
	mSceneManager->getProgramProperties().mFBO->setClearColors(glm::vec4(0.1f, 0.1f, 0.1f, 0.1f));
	if (pInitAll)
		initAll();
}

void Initializer::initAll()
{
	initShaders();
	initTextures();
	initPrimitives();
	initMeshes();
	initMaterial();
	initModels();
	initLights();
	initCrosshair();
	initMousePicker();
	initSkybox();
	initUBO();
	initTerrain();
}

void Initializer::initShaders()
{
	std::string resourcePath = mSceneManager->getProgramProperties().mResourcePath;

	// main shader
	mSceneManager->getProgramProperties().mShaders.pushShader("mainShader", resourcePath + "Shaders/vert.glsl",
																		    resourcePath + "Shaders/frag.glsl");
	// was created for implementing source of lights. like white block
	mSceneManager->getProgramProperties().mShaders.pushShader("singleColorShader", resourcePath + "Shaders/vert.glsl",
																				   resourcePath + "Shaders/shaderSingleColor.glsl");
	// FBO shader
	mSceneManager->getProgramProperties().mShaders.pushShader("FBOshader", resourcePath + "Shaders/vertFrameBuffer.glsl",
																		   resourcePath + "Shaders/fragFrameBuffer.glsl");
	// skybox shader
	mSceneManager->getProgramProperties().mShaders.pushShader("skyboxShader", resourcePath + "Shaders/vertSkybox.glsl",
																			  resourcePath + "Shaders/fragSkybox.glsl");
}

void Initializer::initTextures()
{
	//mSceneManager->getModelProperties().mTextureManager.pushTexture("error",
	//		 std::make_unique<Texture2>(mSceneManager->getProgramProperties().mResourcePath + "error.png", "material.textures"));
}

void Initializer::initPrimitives()
{
	// light block
	mSceneManager->getModelProperties().mPrimitivesManager.pushPrimitive("lightBlock", std::make_shared<Quad>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	// lights on light-posts
	//mSceneManager->getModelProperties().mPrimitivesManager.pushPrimitive("lightPost1", std::make_shared<Quad>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	//mSceneManager->getModelProperties().mPrimitivesManager.pushPrimitive("lightPost2", std::make_shared<Quad>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	//mSceneManager->getModelProperties().mPrimitivesManager.pushPrimitive("lightPost3", std::make_shared<Quad>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	//mSceneManager->getModelProperties().mPrimitivesManager.pushPrimitive("lightPost4", std::make_shared<Quad>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
}

void Initializer::initMeshes()
{
	// light block
	std::weak_ptr<Primitive> lightBlock = mSceneManager->getModelProperties().mPrimitivesManager["lightBlock"];
	mSceneManager->getModelProperties().mFactoryMeshes.pushMesh("lightBlock", std::make_unique<Mesh>(lightBlock));
	
	// light-posts
	//std::weak_ptr<Primitive> light1 = mSceneManager->getModelProperties().mPrimitivesManager["lightPost1"];
	//mSceneManager->getModelProperties().mFactoryMeshes.pushMesh("lightPost1", std::make_unique<Mesh>(light1));
	//std::weak_ptr<Primitive> light2 = mSceneManager->getModelProperties().mPrimitivesManager["lightPost2"];
	//mSceneManager->getModelProperties().mFactoryMeshes.pushMesh("lightPost2", std::make_unique<Mesh>(light2));
	//std::weak_ptr<Primitive> light3 = mSceneManager->getModelProperties().mPrimitivesManager["lightPost3"];
	//mSceneManager->getModelProperties().mFactoryMeshes.pushMesh("lightPost3", std::make_unique<Mesh>(light3));
	//std::weak_ptr<Primitive> light4 = mSceneManager->getModelProperties().mPrimitivesManager["lightPost4"];
	//mSceneManager->getModelProperties().mFactoryMeshes.pushMesh("lightPost4", std::make_unique<Mesh>(light4));
}

void Initializer::initMaterial()
{
	mSceneManager->getMaterialProperties().mMaterial = std::make_unique<Material>(mSceneManager->getMaterialProperties().mAmbient,
																			      mSceneManager->getMaterialProperties().mDiffuse,
																			      mSceneManager->getMaterialProperties().mSpecular,
																			      32.0f);
}

void Initializer::initModels()
{
	// museum
	//mSceneManager->getModelProperties().mModelManager.pushModel("museum", std::make_unique<Model>(glm::vec3(2.0f),
	//												  mSceneManager->getProgramProperties().mResourcePath + "Models/museum.obj"));
	//
	// lamp posts
	//mSceneManager->getModelProperties().mModelManager.pushModel("lampPost1", std::make_unique<Model>(glm::vec3(2.0f),
	//												  mSceneManager->getProgramProperties().mResourcePath + "Models/lamppost.obj"));
	//mSceneManager->getModelProperties().mModelManager.pushModel("lampPost2", std::make_unique<Model>(glm::vec3(2.0f),
	//												  mSceneManager->getProgramProperties().mResourcePath + "Models/lamppost.obj"));
}

void Initializer::initLights()
{
	// lamps in the museum
	//mSceneManager->getLightProperties().mLightManager.pushLight("pointLight1", std::make_unique<PointLight>(glm::vec3(12.0f, 91.0f, -302.0f), 0.5f, 0.045f, 0.075f));
	//mSceneManager->getLightProperties().mLightManager.pushLight("pointLight2", std::make_unique<PointLight>(glm::vec3(-109.0f, 91.0f, -302.0f), 0.5f, 0.045f, 0.075f));
	//mSceneManager->getLightProperties().mLightManager.pushLight("pointLight3", std::make_unique<PointLight>(glm::vec3(5.0f, 94.0f, -724.0f), 0.5f, 0.045f, 0.075f));
	//mSceneManager->getLightProperties().mLightManager.pushLight("pointLight4", std::make_unique<PointLight>(glm::vec3(-117.0f, 94.0f, -724.0f), 0.5f, 0.045f, 0.075f));

	// lampPosts outside
	mSceneManager->getLightProperties().mLightManager.pushLight("lampPost1", std::make_unique<PointLight>(glm::vec3(38.399986, 75.799416, -71.39948), 0.5f, 0.045f, 0.075f));
	//mSceneManager->getLightProperties().mLightManager.pushLight("lampPost2", std::make_unique<PointLight>(glm::vec3(38.399986, 75.799416, -91.09918), 0.5f, 0.045f, 0.075f));
	//mSceneManager->getLightProperties().mLightManager.pushLight("lampPost3", std::make_unique<PointLight>(glm::vec3(-125.79866, 75.899414, -68.599525), 0.5f, 0.045f, 0.075f));
	//mSceneManager->getLightProperties().mLightManager.pushLight("lampPost4", std::make_unique<PointLight>(glm::vec3(-125.79866, 75.899414, -88.19923), 0.5f, 0.045f, 0.075f));
}

void Initializer::initCrosshair()
{
	mSceneManager->getProgramProperties().mCrosshair = std::make_unique<Crosshair>(0.01f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void Initializer::initMousePicker()
{
	//mSceneManager->getProgramProperties().mMousePicker.init(mSceneManager->getProgramProperties().mCamera,
	//											  glm::perspective(glm::radians(45.0f), (float)mSceneManager->getProgramProperties().mWindowWidth /
	//									 		  (float)mSceneManager->getProgramProperties().mWindowHeight, 0.1f, 2000.0f),
	//											  { mSceneManager->getProgramProperties().mWindowWidth, mSceneManager->getProgramProperties().mWindowHeight });
}

void Initializer::initSkybox()
{
	std::string resourcePath = RESOURCES_PATH;
	std::array<std::filesystem::path, 6> paths =
	{
		resourcePath + "cloudy/bluecloud_lf.jpg",
		resourcePath + "cloudy/bluecloud_rt.jpg",
		resourcePath + "cloudy/bluecloud_up.jpg",
		resourcePath + "cloudy/bluecloud_dn.jpg",
		resourcePath + "cloudy/bluecloud_ft.jpg",
		resourcePath + "cloudy/bluecloud_bk.jpg"
	};
	mSceneManager->getProgramProperties().mShaders["skyboxShader"].bind();
	mSceneManager->getProgramProperties().mSkybox = std::make_unique<Skybox>(typeSkybox::SPHERE, paths, 0);
	mSceneManager->getProgramProperties().mShaders["skyboxShader"].setUniform1i("uSkybox", 0);
}

void Initializer::initUBO()
{
	mSceneManager->getProgramProperties().mUBO.init({ {mSceneManager->getProgramProperties().mShaders["mainShader"].getID(), "Matrices"}},
										  0, 2 * sizeof(glm::mat4));
}

void Initializer::initTerrain()	
{
	std::string resourcePath = mSceneManager->getProgramProperties().mResourcePath;
	std::vector<std::filesystem::path> paths =
	{
		resourcePath + "sand.png",
		resourcePath + "grass.png",
		resourcePath + "stone.png",
		resourcePath + "snow.png"
	};
	
	float worldScale = 4.0f;
	float textureScale = 16.0f;
	mSceneManager->getModelProperties().mTerrain = std::make_unique<FractalNoiseTerrain>();
	mSceneManager->getModelProperties().mTerrain->init(worldScale, textureScale, paths);
	//mSceneManager->getModelProperties().mTerrain->loadFromFile(resourcePath + "terrain.png");
	
	uint32_t size = 513;        // Power of 2 + 1 works better
	uint32_t octaves = 4;
	float minHeight = 0.0f;
	float maxHeight = 200.0f;
	float amplitude = 50.0f;    // Much higher initial amplitude
	float frequency = 0.1f;    // Lower frequency for broader features
	float lacunarity = 2.0f;    // Standard
	float persistence = 0.5f;

	//float roughness = 2.0f;
	
	// TODO: THE PROBLEM OF TERRAIN IN THE HEIGHT GENERATION!!! ABT MidPointDisp
	reinterpret_cast<FractalNoiseTerrain*>(mSceneManager->getModelProperties().mTerrain.get())->setLight(glm::vec3(1.0f, -1.0f, 0.0f), 0.5f);
	reinterpret_cast<FractalNoiseTerrain*>(mSceneManager->getModelProperties().mTerrain.get())->init(size, minHeight, maxHeight, amplitude, 
																									 frequency, octaves, 
																									 lacunarity, persistence);
	//reinterpret_cast<MidpointDispTerrain*>(mSceneManager->getModelProperties().mTerrain.get())->setPos(glm::vec3(-200.0f, -400.0f, -200.0f));
	mSceneManager->getModelProperties().mTerrain->setHeights(maxHeight - 200.0f, maxHeight - 150.0f, maxHeight - 100.0f, maxHeight - 50.0f);
	mSceneManager->getModelProperties().mTerrain->setOneColor(false);
}

/*
	octave - how many iterations we should do for the grid
	amplitude - how extreme our grid needs to be 
	frequency - how much detail
	lacunarity - how much we should increase our frequency per octave?
	persistence - how quickly our grid should shrink per octave?
*/