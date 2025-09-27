#include "Initializer.h"
#include "Program.h"

Initializer::Initializer(Program* pProgram)
{
	mProgram = pProgram;

	// resource path
	mProgram->mProgramProperties.mResourcePath = RESOURCES_PATH;

	// camera
	mProgram->mProgramProperties.mCamera.setSensivity(0.2f);

	// projection matrix
	mProgram->mModelProperties.mProjMatrix = glm::perspective(glm::radians(45.0f), (float)mProgram->mProgramProperties.mWindowWidth /
														     (float)mProgram->mProgramProperties.mWindowHeight, 0.1f, 2000.0f);
	// fbo
	mProgram->mProgramProperties.mFBO.init(mProgram->mProgramProperties.mWindowWidth, mProgram->mProgramProperties.mWindowHeight);
	mProgram->mProgramProperties.mFBO.setClearColors({ 0.1f, 0.1f, 0.1f, 0.1f });

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
}

void Initializer::initShaders()
{
	std::string resourcePath = mProgram->mProgramProperties.mResourcePath;

	// main shader
	mProgram->mProgramProperties.mShaders.pushShader("mainShader", resourcePath + "Shaders/vert.glsl",
																  resourcePath + "Shaders/frag.glsl");
	// was created for implementing source of lights. like white block
	mProgram->mProgramProperties.mShaders.pushShader("singleColorShader", resourcePath + "Shaders/vert.glsl",
																		 resourcePath + "Shaders/shaderSingleColor.glsl");
	// FBO shader
	mProgram->mProgramProperties.mShaders.pushShader("FBOshader", resourcePath + "Shaders/vertFrameBuffer.glsl",
																 resourcePath + "Shaders/fragFrameBuffer.glsl");
	// skybox shader
	mProgram->mProgramProperties.mShaders.pushShader("skyboxShader", resourcePath + "Shaders/vertSkybox.glsl",
																	resourcePath + "Shaders/fragSkybox.glsl");
}

void Initializer::initTextures()
{
	//mProgram->mModelProperties.mTextureManager.pushTexture("error",
	//		 std::make_unique<Texture2>(mProgram->mProgramProperties.mResourcePath + "error.png", "material.textures"));
}

void Initializer::initPrimitives()
{
	// floor 
	mProgram->mModelProperties.mPrimitivesManager.pushPrimitive("floor", std::make_shared<Quad>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	// light block
	mProgram->mModelProperties.mPrimitivesManager.pushPrimitive("lightBlock", std::make_shared<Quad>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	// lights on light-posts
	mProgram->mModelProperties.mPrimitivesManager.pushPrimitive("lightPost1", std::make_shared<Quad>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	mProgram->mModelProperties.mPrimitivesManager.pushPrimitive("lightPost2", std::make_shared<Quad>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	mProgram->mModelProperties.mPrimitivesManager.pushPrimitive("lightPost3", std::make_shared<Quad>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	mProgram->mModelProperties.mPrimitivesManager.pushPrimitive("lightPost4", std::make_shared<Quad>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
}

void Initializer::initMeshes()
{
	// floor
	std::weak_ptr<Primitive> floor = mProgram->mModelProperties.mPrimitivesManager["floor"];
	mProgram->mModelProperties.mFactoryMeshes.pushMesh("floor", std::make_unique<Mesh>(floor));

	// light block
	std::weak_ptr<Primitive> lightBlock = mProgram->mModelProperties.mPrimitivesManager["lightBlock"];
	mProgram->mModelProperties.mFactoryMeshes.pushMesh("lightBlock", std::make_unique<Mesh>(lightBlock));
	
	// light-posts
	std::weak_ptr<Primitive> light1 = mProgram->mModelProperties.mPrimitivesManager["lightPost1"];
	mProgram->mModelProperties.mFactoryMeshes.pushMesh("lightPost1", std::make_unique<Mesh>(light1));
	std::weak_ptr<Primitive> light2 = mProgram->mModelProperties.mPrimitivesManager["lightPost2"];
	mProgram->mModelProperties.mFactoryMeshes.pushMesh("lightPost2", std::make_unique<Mesh>(light2));
	std::weak_ptr<Primitive> light3 = mProgram->mModelProperties.mPrimitivesManager["lightPost3"];
	mProgram->mModelProperties.mFactoryMeshes.pushMesh("lightPost3", std::make_unique<Mesh>(light3));
	std::weak_ptr<Primitive> light4 = mProgram->mModelProperties.mPrimitivesManager["lightPost4"];
	mProgram->mModelProperties.mFactoryMeshes.pushMesh("lightPost4", std::make_unique<Mesh>(light4));
}

void Initializer::initMaterial()
{
	mProgram->mMaterialProperties.mMaterial = std::make_unique<Material>(mProgram->mMaterialProperties.mAmbient,
																	    mProgram->mMaterialProperties.mDiffuse,
																	    mProgram->mMaterialProperties.mSpecular,
																	    32.0f);
}

void Initializer::initModels()
{
	// museum
	mProgram->mModelProperties.mModelManager.pushModel("museum", std::make_unique<Model>(glm::vec3(2.0f),
													  mProgram->mProgramProperties.mResourcePath + "Models/museum.obj"));

	// lamp posts
	mProgram->mModelProperties.mModelManager.pushModel("lampPost1", std::make_unique<Model>(glm::vec3(2.0f),
													  mProgram->mProgramProperties.mResourcePath + "Models/lamppost.obj"));
	mProgram->mModelProperties.mModelManager.pushModel("lampPost1", std::make_unique<Model>(glm::vec3(2.0f),
													  mProgram->mProgramProperties.mResourcePath + "Models/lamppost.obj"));
}

void Initializer::initLights()
{
	// lamps in the museum
	mProgram->mLightProperties.mLightManager.pushLight("pointLight1", std::make_unique<PointLight>(glm::vec3(12.0f, 91.0f, -302.0f), 0.5f, 0.045f, 0.075f));
	mProgram->mLightProperties.mLightManager.pushLight("pointLight2", std::make_unique<PointLight>(glm::vec3(-109.0f, 91.0f, -302.0f), 0.5f, 0.045f, 0.075f));
	mProgram->mLightProperties.mLightManager.pushLight("pointLight3", std::make_unique<PointLight>(glm::vec3(5.0f, 94.0f, -724.0f), 0.5f, 0.045f, 0.075f));
	mProgram->mLightProperties.mLightManager.pushLight("pointLight4", std::make_unique<PointLight>(glm::vec3(-117.0f, 94.0f, -724.0f), 0.5f, 0.045f, 0.075f));

	// lampPosts outside
	mProgram->mLightProperties.mLightManager.pushLight("lampPost1", std::make_unique<PointLight>(glm::vec3(38.399986, 75.799416, -71.39948), 0.5f, 0.045f, 0.075f));
	mProgram->mLightProperties.mLightManager.pushLight("lampPost2", std::make_unique<PointLight>(glm::vec3(38.399986, 75.799416, -91.09918), 0.5f, 0.045f, 0.075f));
	mProgram->mLightProperties.mLightManager.pushLight("lampPost3", std::make_unique<PointLight>(glm::vec3(-125.79866, 75.899414, -68.599525), 0.5f, 0.045f, 0.075f));
	mProgram->mLightProperties.mLightManager.pushLight("lampPost4", std::make_unique<PointLight>(glm::vec3(-125.79866, 75.899414, -88.19923), 0.5f, 0.045f, 0.075f));
}

void Initializer::initCrosshair()
{
	mProgram->mProgramProperties.mCrosshair = std::make_unique<Crosshair>(0.01f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void Initializer::initMousePicker()
{
	mProgram->mProgramProperties.mMousePicker.init(mProgram->mProgramProperties.mCamera,
												  glm::perspective(glm::radians(45.0f), (float)mProgram->mProgramProperties.mWindowWidth /
										 		  (float)mProgram->mProgramProperties.mWindowHeight, 0.1f, 2000.0f),
												  { mProgram->mProgramProperties.mWindowWidth, mProgram->mProgramProperties.mWindowHeight });
}

void Initializer::initSkybox()
{
	std::string resourcePath = RESOURCES_PATH;
	std::array<std::filesystem::path, 6> paths =
	{
		resourcePath + "ulukai/corona_lf.png",
		resourcePath + "ulukai/corona_rt.png",
		resourcePath + "ulukai/corona_up.png",
		resourcePath + "ulukai/corona_dn.png",
		resourcePath + "ulukai/corona_ft.png",
		resourcePath + "ulukai/corona_bk.png"
	};
	mProgram->mProgramProperties.mShaders["skyboxShader"].bind();
	mProgram->mProgramProperties.mSkybox = std::make_unique<Skybox>(typeSkybox::SPHERE, paths, 0);
	mProgram->mProgramProperties.mShaders["skyboxShader"].setUniform1i("uSkybox", 0);
}

void Initializer::initUBO()
{
	mProgram->mProgramProperties.mUBO.init({ {mProgram->mProgramProperties.mShaders["mainShader"].getID(), "Matrices"}},
										  0, 2 * sizeof(glm::mat4));
}
