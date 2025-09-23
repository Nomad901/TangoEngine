#include "Program.h"

Program::Program(uint32_t pWindowWidth, uint32_t pWindowHeight)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cout << "Couldnt initialize the window!\n";
		exit(1);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	mProgramProperties.mWindowWidth = pWindowWidth;
	mProgramProperties.mWindowHeight = pWindowHeight;

	mProgramProperties.mWindow = SDL_CreateWindow("Batch Renderer", pWindowWidth, pWindowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!mProgramProperties.mWindow)
		std::cout << "Couldnt set the window!\n";
	mProgramProperties.mContext = SDL_GL_CreateContext(mProgramProperties.mWindow);
	if (!mProgramProperties.mContext)
		std::cout << "Couldnt set the context!\n";
	if (!SDL_GL_MakeCurrent(mProgramProperties.mWindow, mProgramProperties.mContext))
		std::cout << "Couldnt set the window and the context current!\n";
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		std::cout << "Couldnt load the glad loader!\n";
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	mUI.init(mProgramProperties.mWindow, mProgramProperties.mContext);
}

Program::~Program()
{
	SDL_GL_DestroyContext(mProgramProperties.mContext);
	SDL_DestroyWindow(mProgramProperties.mWindow);
	SDL_Quit();
}

void Program::run()
{
	mProgramProperties.mResourcePath = RESOURCES_PATH;
	initAll();

	mProgramProperties.mCamera.setSensivity(0.2f);

	mModelProperties.mProjMatrix = glm::perspective(glm::radians(45.0f), (float)mProgramProperties.mWindowWidth / 
																		 (float)mProgramProperties.mWindowHeight, 0.1f, 2000.0f);
	mProgramProperties.mFBO.init(800, 600);
	mProgramProperties.mFBO.setClearColors({ 0.1f, 0.1f, 0.1f, 0.1f });

	float prevTime = 0.0f;
	float currTime = 0.0f;
	float timeDiff;
	uint32_t counter = 0;

	while (mProgramProperties.mProgIsRunning)
	{
		float beginFrame = SDL_GetTicks();

		currTime = SDL_GetTicks();
		timeDiff = currTime - prevTime;
		counter++;
		if (timeDiff >= 1.0f / 100.0f)
		{
			std::string fps = std::to_string((1.0f / timeDiff) * counter * 1000);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = std::format("Museum of lights. FPS: {} | MS: {}", fps, ms);
			SDL_SetWindowTitle(mProgramProperties.mWindow, newTitle.c_str());
			prevTime = currTime;
			counter = 0;
		}

		input();
		preDraw();
		draw();

		float deltaTime = SDL_GetTicks() - beginFrame;
		if (deltaTime < 8)
			SDL_Delay(8 - deltaTime);

		SDL_GL_SwapWindow(mProgramProperties.mWindow);
	}
}

void Program::input()
{
	while (SDL_PollEvent(&mProgramProperties.mEvent))
	{
		SDL_Event event = mProgramProperties.mEvent;

		ImGui_ImplSDL3_ProcessEvent(&event);

		if (event.type == SDL_EVENT_QUIT ||
			event.key.key == SDLK_ESCAPE)
		{
			mProgramProperties.mProgIsRunning = false;
			break;
		}

		// KEYS CHECKERS
		if (event.type == SDL_EVENT_KEY_DOWN)
			mProgramProperties.mKeyCodes[event.key.key] = true;
		if (event.type == SDL_EVENT_KEY_UP)
			mProgramProperties.mKeyCodes[event.key.key] = false;

		// CAMERA MOVING
		if (event.type == SDL_EVENT_MOUSE_MOTION && mProgramProperties.mTakeCursor)
			mProgramProperties.mCamera.mouseMovement(glm::vec2(event.motion.xrel, event.motion.yrel));
	}

	controlScreen();
	controlCamera();
	controlModel();
	controlLight();
}

void Program::preDraw()
{
	takerCursor();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	mUI.control(*this);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// optimization
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_PROGRAM_POINT_SIZE);

	if(mProgramProperties.mWireFrameMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glViewport(0, 0, mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight);
	if (mProgramProperties.mWhiteScreen)
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	else
		glClearColor(0.20f, 0.20f, 0.20f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_STENCIL_TEST);

	glm::vec2 mousePos;
	SDL_GetMouseState(&mousePos.x, &mousePos.y);
	mProgramProperties.mMousePicker.update(mProgramProperties.mCamera, { mousePos.x, mousePos.y });

	setLights();
	setLightCube();

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	mProgramProperties.mShader.bind();
	setModels();
}

void Program::draw()
{
	ImGui::EndFrame();

	//mModel[0]->render(mShader);
	//mFactoryMeshes.render();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Program::initAll()
{
	initShaders();
	initTextures();
	initSkybox();
	initPrimitives();
	initMeshes();
	initMaterial();
	initModels();
	initLights();
	initCrosshair();
	initMousePicker();
	initUBO();
}

void Program::initShaders()
{
	// main shader
	mProgramProperties.mShader.init(mProgramProperties.mShader.getResourcePath() + "Shaders/vert.glsl",
									mProgramProperties.mShader.getResourcePath() + "Shaders/frag.glsl");

	// was create for creating source of lights. like white block
	mProgramProperties.mShaderSingleColor.init(mProgramProperties.mShader.getResourcePath() + "Shaders/vert.glsl",
											   mProgramProperties.mShader.getResourcePath() + "Shaders/shaderSingleColor.glsl");

	// FBO shader
	mProgramProperties.mShaderSecondScreen.init(mProgramProperties.mShaderSecondScreen.getResourcePath() + "Shaders/vertFrameBuffer.glsl",
												mProgramProperties.mShader.getResourcePath() + "Shaders/fragFrameBuffer.glsl");

	// skybox shader
	mProgramProperties.mSkyboxShader.init(mProgramProperties.mSkyboxShader.getResourcePath() + "Shaders/vertSkybox.glsl",
										  mProgramProperties.mSkyboxShader.getResourcePath() + "Shaders/fragSkybox.glsl");

	// skybox block shader
	mProgramProperties.mSkyboxBlockShader.init(mProgramProperties.mSkyboxBlockShader.getResourcePath() + "Shaders/skyboxBlockVert.glsl",
											   mProgramProperties.mSkyboxBlockShader.getResourcePath() + "Shaders/skyboxBlockFrag.glsl");

	// shader of normals
	mProgramProperties.mShaderNormals.init(mProgramProperties.mSkyboxBlockShader.getResourcePath() + "Shaders/normalsVert.glsl",
										   mProgramProperties.mSkyboxBlockShader.getResourcePath() + "Shaders/normalsFrag.glsl",
										   mProgramProperties.mSkyboxBlockShader.getResourcePath() + "Shaders/normalsGeom.geom");
}

void Program::initTextures()
{
	mModelProperties.mTextures.reserve(32);
	mModelProperties.mTextures.emplace_back(mProgramProperties.mResourcePath + "blending_transparent_window.png", "material.textures");
	//mModelProperties.mTextures.emplace_back(mProgramProperties.mResourcePath + "grass.png", "material.textures");
}

void Program::initPrimitives()
{
	// the room
	mModelProperties.mPrimitives.insert_or_assign("museum", std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	// floor 
	mModelProperties.mPrimitives.insert_or_assign("floor", std::make_shared<Quad>(mModelProperties.mTextures[0], 0));

	// light block
	mModelProperties.mPrimitives.insert_or_assign("lightBlock", std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	// character
	mModelProperties.mPrimitives.insert_or_assign("character", std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	// blocks to choose
	mModelProperties.mPrimitives.insert_or_assign("block1", std::make_shared<Cube>(mProgramProperties.mSkybox->getTextures(), 1, false));
	mModelProperties.mPrimitives.insert_or_assign("block2", std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	// lights on ligth-posts
	mModelProperties.mPrimitives.insert_or_assign("lightPost1", std::make_shared<Quad>(mModelProperties.mTextures[0], 0));
	mModelProperties.mPrimitives.insert_or_assign("lightPost2", std::make_shared<Quad>(mModelProperties.mTextures[0], 0));
	mModelProperties.mPrimitives.insert_or_assign("lightPost3", std::make_shared<Quad>(mModelProperties.mTextures[0], 0));
	mModelProperties.mPrimitives.insert_or_assign("lightPost4", std::make_shared<Quad>(mModelProperties.mTextures[0], 0));

	// test quad
	mModelProperties.mPrimitives.insert_or_assign("testQuad", std::make_shared<Quad>(mModelProperties.mTextures[0], 0));
}

void Program::initMeshes()
{
	// the room
	std::weak_ptr<Primitive> museum = mModelProperties.mPrimitives["museum"];
	mModelProperties.mFactoryMeshes.pushMesh("Room", std::make_unique<Mesh>(museum));

	// light block
	std::weak_ptr<Primitive> lightBlock = mModelProperties.mPrimitives["lightBlock"];
	mModelProperties.mFactoryMeshes.pushMesh("lightBlock", std::make_unique<Mesh>(lightBlock));

	// floor
	std::weak_ptr<Primitive> floor = mModelProperties.mPrimitives["floor"];
	mModelProperties.mFactoryMeshes.pushMesh("floor", std::make_unique<Mesh>(floor));

	// character
	std::weak_ptr<Primitive> character = mModelProperties.mPrimitives["character"];
	mModelProperties.mFactoryMeshes.pushMesh("character", std::make_unique<Mesh>(character));

	// blocks to choose
	std::weak_ptr<Primitive> block1 = mModelProperties.mPrimitives["block1"];
	mModelProperties.mFactoryMeshes.pushMesh("block1", std::make_unique<Mesh>(block1));
	std::weak_ptr<Primitive> block2 = mModelProperties.mPrimitives["block2"];
	mModelProperties.mFactoryMeshes.pushMesh("block2", std::make_unique<Mesh>(block2));

	// light-posts
	std::weak_ptr<Primitive> light1 = mModelProperties.mPrimitives["lightPost1"];
	mModelProperties.mFactoryMeshes.pushMesh("lightPost1", std::make_unique<Mesh>(light1));
	std::weak_ptr<Primitive> light2 = mModelProperties.mPrimitives["lightPost2"];
	mModelProperties.mFactoryMeshes.pushMesh("lightPost2", std::make_unique<Mesh>(light2));
	std::weak_ptr<Primitive> light3 = mModelProperties.mPrimitives["lightPost3"];
	mModelProperties.mFactoryMeshes.pushMesh("lightPost3", std::make_unique<Mesh>(light3));
	std::weak_ptr<Primitive> light4 = mModelProperties.mPrimitives["lightPost4"];
	mModelProperties.mFactoryMeshes.pushMesh("lightPost4", std::make_unique<Mesh>(light4));

	// test quad
	std::weak_ptr<Primitive> testQuad = mModelProperties.mPrimitives["testQuad"];
	mModelProperties.mFactoryMeshes.pushMesh("testQuad", std::make_unique<Mesh>(testQuad));
}

void Program::initMaterial()
{
	mMaterialProperties.mMaterial = std::make_unique<Material>(mMaterialProperties.mAmbient,
		mMaterialProperties.mDiffuse,
		mMaterialProperties.mSpecular,
		32.0f, mModelProperties.mTextures);
}

void Program::initModels()
{
	mModelProperties.mModel.push_back(std::make_unique<Model>(glm::vec3(2.0f), mMaterialProperties.mMaterial.get(),
														      mProgramProperties.mResourcePath + "Models/museum.obj", 
															  std::vector<Texture2>()));
	mModelProperties.mModel.push_back(std::make_unique<Model>(glm::vec3(2.0f), mMaterialProperties.mMaterial.get(),
													          mProgramProperties.mResourcePath + "Models/mirror.obj", 
														      std::vector<Texture2>()));
	mModelProperties.mModel.push_back(std::make_unique<Model>(glm::vec3(2.0f), mMaterialProperties.mMaterial.get(),
															  mProgramProperties.mResourcePath + "Models/lamppost.obj",
															  std::vector<Texture2>()));
	mModelProperties.mModel.push_back(std::make_unique<Model>(glm::vec3(2.0f), mMaterialProperties.mMaterial.get(),
															  mProgramProperties.mResourcePath + "Models/lamppost.obj",
															  std::vector<Texture2>()));
}

void Program::initLights()
{
	// lamps in the museum
	mLightProperties.mLightManager.pushLight("pointLight1", std::make_unique<PointLight>(glm::vec3(12.0f, 91.0f, -302.0f), 0.5f, 0.045f, 0.075f));
	mLightProperties.mLightManager.pushLight("pointLight2", std::make_unique<PointLight>(glm::vec3(-109.0f, 91.0f, -302.0f), 0.5f, 0.045f, 0.075f));
	mLightProperties.mLightManager.pushLight("pointLight3", std::make_unique<PointLight>(glm::vec3(5.0f, 94.0f, -724.0f), 0.5f, 0.045f, 0.075f));
	mLightProperties.mLightManager.pushLight("pointLight4", std::make_unique<PointLight>(glm::vec3(-117.0f, 94.0f, -724.0f), 0.5f, 0.045f, 0.075f));

	// lampPosts outside
	mLightProperties.mLightManager.pushLight("lampPost1", std::make_unique<PointLight>(glm::vec3(38.399986, 75.799416, -71.39948), 0.5f, 0.045f, 0.075f));
	mLightProperties.mLightManager.pushLight("lampPost2", std::make_unique<PointLight>(glm::vec3(38.399986, 75.799416, -91.09918), 0.5f, 0.045f, 0.075f));
	mLightProperties.mLightManager.pushLight("lampPost3", std::make_unique<PointLight>(glm::vec3(-125.79866, 75.899414, -68.599525), 0.5f, 0.045f, 0.075f));
	mLightProperties.mLightManager.pushLight("lampPost4", std::make_unique<PointLight>(glm::vec3(-125.79866, 75.899414, -88.19923), 0.5f, 0.045f, 0.075f));
}

void Program::initCrosshair()
{
	mProgramProperties.mCrosshair = std::make_unique<Crosshair>(0.01f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void Program::initMousePicker()
{
	mProgramProperties.mMousePicker.init(mProgramProperties.mCamera,
		glm::perspective(glm::radians(45.0f), (float)mProgramProperties.mWindowWidth /
			(float)mProgramProperties.mWindowHeight, 0.1f, 2000.0f),
		{ mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight });
}

void Program::initSkybox()
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
	mProgramProperties.mSkyboxShader.bind();
	mProgramProperties.mSkybox = std::make_unique<Skybox>(typeSkybox::CUBE, paths, 1);
	mProgramProperties.mSkyboxShader.setUniform1i("uSkybox", 1);
}

void Program::initUBO()
{
	mProgramProperties.mUBO.init({ {mProgramProperties.mShader.getID(), "Matrices"},
							       {mProgramProperties.mSkyboxBlockShader.getID(), "Matrices"} },
								    0, 2 * sizeof(glm::mat4));
}

void Program::controlScreen()
{
	// WHITE/BLACK SCREEN
	if (mProgramProperties.mKeyCodes[SDLK_V])
		mProgramProperties.mWhiteScreen = true;
	if (mProgramProperties.mKeyCodes[SDLK_B])
		mProgramProperties.mWhiteScreen = false;

	// CURSOR
	if (mProgramProperties.mKeyCodes[SDLK_Z])
		mProgramProperties.mTakeCursor = !mProgramProperties.mTakeCursor;
}

void Program::controlCamera()
{
	float speed = 1.0f;
	if (mProgramProperties.mKeyCodes[SDLK_LSHIFT])
		speed = 2.0f;
	if (mProgramProperties.mKeyCodes[SDLK_W])
		mProgramProperties.mCamera.moveCamera(moveSides::FORWARD, speed);
	if (mProgramProperties.mKeyCodes[SDLK_S])
		mProgramProperties.mCamera.moveCamera(moveSides::BACKWARD, speed);
	if (mProgramProperties.mKeyCodes[SDLK_A])
		mProgramProperties.mCamera.moveCamera(moveSides::LEFT, speed);
	if (mProgramProperties.mKeyCodes[SDLK_D])
		mProgramProperties.mCamera.moveCamera(moveSides::RIGHT, speed);
	if (mProgramProperties.mKeyCodes[SDLK_LCTRL])
		mProgramProperties.mCamera.moveCamera(moveSides::DOWN, speed);
	if (mProgramProperties.mKeyCodes[SDLK_SPACE])
		mProgramProperties.mCamera.moveCamera(moveSides::UP, speed);
}

void Program::controlModel()
{
	// ROTATE DEGREE
	if (mProgramProperties.mKeyCodes[SDLK_R])
		mModelProperties.mRotateDegree -= 0.4f;
	if (mProgramProperties.mKeyCodes[SDLK_F])
		mModelProperties.mRotateDegree += 0.4f;
}

void Program::controlLight()
{
	// MOVING LIGHT
	float speedMovingLight = 0.1f;
	if (!mModelProperties.mRotatedMode)
	{
		if (mProgramProperties.mKeyCodes[SDLK_LEFT])
			mLightProperties.mPosLight.x -= speedMovingLight;
		if (mProgramProperties.mKeyCodes[SDLK_RIGHT])
			mLightProperties.mPosLight.x += speedMovingLight;
		if (mProgramProperties.mKeyCodes[SDLK_UP])
			mLightProperties.mPosLight.y += speedMovingLight;
		if (mProgramProperties.mKeyCodes[SDLK_DOWN])
			mLightProperties.mPosLight.y -= speedMovingLight;
		if (mProgramProperties.mKeyCodes[SDLK_RCTRL])
			mLightProperties.mPosLight.z -= speedMovingLight;
		if (mProgramProperties.mKeyCodes[SDLK_RSHIFT])
			mLightProperties.mPosLight.z += speedMovingLight;
	}
}

void Program::takerCursor()
{
	if (mProgramProperties.mTakeCursor)
	{
		SDL_WarpMouseInWindow(mProgramProperties.mWindow,
			mProgramProperties.mWindowWidth / 2,
			mProgramProperties.mWindowHeight / 2);
		SDL_SetWindowRelativeMouseMode(mProgramProperties.mWindow, true);
	}
	else
		SDL_SetWindowRelativeMouseMode(mProgramProperties.mWindow, false);
}

void Program::setLights()
{
	setMaterials();

	mLightProperties.mLightManager.sendAllToShader(mProgramProperties.mShader, mMaterialProperties.mMaterial.get(), true);
}

void Program::setLightCube()
{

}

void Program::setMaterials()
{
	mMaterialProperties.mMaterial->setAmbient(mMaterialProperties.mAmbient);
	mMaterialProperties.mMaterial->setDiffuse(mMaterialProperties.mDiffuse);
	mMaterialProperties.mMaterial->setSpecular(mMaterialProperties.mSpecular);
	mMaterialProperties.mMaterial->setShines(mMaterialProperties.mShines);
}

void Program::setModels()
{
	glStencilMask(0x00);

	mProgramProperties.mFBO.bind();
	glViewport(0, 0, 800, 600);
	mProgramProperties.mFBO.setClearColors({ 0.7f, 0.7f, 0.7f, 0.7f });
	mProgramProperties.mFBO.clearColor();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	setSkybox();	
	mProgramProperties.mCamera.setYaw(mProgramProperties.mCamera.getYaw() + 180.0f);
	mProgramProperties.mCamera.mouseMovement({ 0, 0 }, false);
	drawModels();
	mProgramProperties.mCamera.setYaw(mProgramProperties.mCamera.getYaw() - 180.0f);
	mProgramProperties.mCamera.mouseMovement({ 0, 0 }, true);
	drawNormals();

	mProgramProperties.mFBO.unbind();
	glViewport(0, 0, mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight);
	mProgramProperties.mFBO.setClearColors({ 0.1f, 0.1f, 0.1f, 0.1f });
	mProgramProperties.mFBO.clearColor();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glStencilMask(0x00);

	setSkybox();
	drawModels();
	drawNormals();
	
	glDisable(GL_DEPTH_TEST);
	mProgramProperties.mCrosshair->render(mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight);

	mProgramProperties.mShaderSecondScreen.bind();
	mModelProperties.mFactoryMeshes.getMesh("testQuad").initMVP(mModelProperties.mProjMatrix,
																mProgramProperties.mCamera.getViewMatrix(),
																glm::vec3(-3.0f, 30.0f, -150.0f),
																std::make_pair(-180.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
																glm::vec3(25.0f, 78.0f, 1.0f));
	mProgramProperties.mShaderSecondScreen.setMatrixUniform4fv("uMVP", mModelProperties.mFactoryMeshes.getMesh("testQuad").getMVP());

	mProgramProperties.mShaderSecondScreen.setUniform1i("uTexture", 0);
	mProgramProperties.mShaderSecondScreen.setUniform1f("surroundNum", surroundNum);
	mProgramProperties.mShaderSecondScreen.setUniform1f("insideNum", insideNum);

	mModelProperties.mFactoryMeshes.getMesh("testQuad").drawInFrameBuffer(mProgramProperties.mFBO.getTexture());
}

void Program::setSkybox()
{
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);

	mProgramProperties.mSkyboxShader.bind();
	static float pNumber = 1.0f;
	glm::mat4 model = model = glm::scale(glm::vec3(3000.0f, 3000.0f, 3000.0f));
	glm::mat4 view = glm::mat4(glm::mat3(mProgramProperties.mCamera.getViewMatrix()));
	view = glm::rotate(view, glm::radians(pNumber), glm::vec3(0.0f, 1.0f, 0.0f));
	pNumber += 0.01f;
	glm::mat4 skyboxMVP = mModelProperties.mProjMatrix * view * model;

	mProgramProperties.mSkyboxShader.setMatrixUniform4fv("uMVP", skyboxMVP);

	mProgramProperties.mSkybox->getMesh().drawSkybox();

	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
}

void Program::drawModels()
{
	mProgramProperties.mUBO.appendData(0, glm::value_ptr(mModelProperties.mProjMatrix));
	mProgramProperties.mUBO.appendData(sizeof(glm::mat4), glm::value_ptr(mProgramProperties.mCamera.getViewMatrix()));
	
	mProgramProperties.mShader.bind();
	mProgramProperties.mShader.setUniform3fv("cameraPos", mProgramProperties.mCamera.getPos());
	mMaterialProperties.mMaterial->sendToShader(mProgramProperties.mShader, true);

	//// floor
	mModelProperties.mFactoryMeshes.getMesh("floor").initMVP(mModelProperties.mProjMatrix,
															 mProgramProperties.mCamera.getViewMatrix(),
															 glm::vec3(1.0f, 1.0f, -23.0f),
															 std::make_pair(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
															 glm::vec3(3000.0f, 3000.0f, 1.0f));
	mModelProperties.mFactoryMeshes.getMesh("floor").setUniforms(mProgramProperties.mShader, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	mModelProperties.mFactoryMeshes.getMesh("floor").draw();

	//// museum
	mModelProperties.mModel[0]->initMVP(mModelProperties.mProjMatrix,
										mProgramProperties.mCamera.getViewMatrix(),
										glm::vec3(1.0f, -34.5f, -1147.0f),
										std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
										glm::vec3(1.0f, 1.0f, 1.0f));
	mModelProperties.mModel[0]->setUniforms(mProgramProperties.mShader, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	mModelProperties.mModel[0]->render();

	//// light posts

	auto setLightModels = [&](const glm::vec3& pPos, int32_t pInd)
		{
			mModelProperties.mModel[pInd]->initMVP(mModelProperties.mProjMatrix, 
												   mProgramProperties.mCamera.getViewMatrix(),
												   pPos, std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
												   glm::vec3(20.0f, 20.0f, 20.0f));
			mModelProperties.mModel[pInd]->setUniforms(mProgramProperties.mShader, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			mModelProperties.mModel[pInd]->render();

		};
	auto setLightLights = [&](const glm::vec3& pPos, std::string_view pName)
		{
			mModelProperties.mFactoryMeshes.getMesh(pName).initMVP(mModelProperties.mProjMatrix,
																   mProgramProperties.mCamera.getViewMatrix(),
																   pPos, std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
																   glm::vec3(5.0f, 5.0f, 5.0f));
			mModelProperties.mFactoryMeshes.getMesh(pName).setUniforms(mProgramProperties.mShader, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			mModelProperties.mFactoryMeshes.getMesh(pName).draw();
		};
	setLightModels(glm::vec3( 1.0f, -20.0f, 150.0f), 1);
	setLightModels(glm::vec3( 22.900051, 25.700062, -78.89937), 2);
	setLightModels(glm::vec3(-140.900051, 25.700062, -78.89937), 3);

	setLightLights(glm::vec3( 38.399986, 75.799416, -69.39948), "lightPost1");
	setLightLights(glm::vec3( 38.399986, 75.799416, -88.09918), "lightPost2");
	setLightLights(glm::vec3(-125.79866, 75.899414, -68.599525), "lightPost3");
	setLightLights(glm::vec3(-125.79866, 75.899414, -88.19923), "lightPost4");

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	mModelProperties.mFactoryMeshes.getMesh("block2").initMVP(mModelProperties.mProjMatrix,
															  mProgramProperties.mCamera.getViewMatrix(),
															  glm::vec3(10.0f, 10.0f, 10.0f),
															  std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
															  glm::vec3(5.0f, 5.0f, 5.0f));
	mModelProperties.mFactoryMeshes.getMesh("block2").setUniforms(mProgramProperties.mShader, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	mModelProperties.mFactoryMeshes.getMesh("block2").draw();


	mMaterialProperties.mMaterial->sendToShader(mProgramProperties.mShader, false);
	mProgramProperties.mSkyboxBlockShader.bind();

	mModelProperties.mFactoryMeshes.getMesh("block1").initMVP(mModelProperties.mProjMatrix, 
															  mProgramProperties.mCamera.getViewMatrix(),
															  glm::vec3(1.0f, 1.0f, 1.0f),
															  std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
															  glm::vec3(5.0f, 5.0f, 5.0f));
	mProgramProperties.mSkyboxBlockShader.setMatrixUniform4fv("uMVP", mModelProperties.mFactoryMeshes.getMesh("block1").getMVP());
	mProgramProperties.mSkyboxBlockShader.setMatrixUniform4fv("uModel", mModelProperties.mFactoryMeshes.getMesh("block1").getModelMatrix());
	mProgramProperties.mSkyboxBlockShader.setUniform3fv("uCameraPos", mProgramProperties.mCamera.getPos());
	mProgramProperties.mSkyboxBlockShader.setUniform1i("uSkybox", 1);

	mModelProperties.mFactoryMeshes.getMesh("block1").drawSkybox();

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	mProgramProperties.mShaderSingleColor.bind();
	float scale = 5.1f;
	if (mProgramProperties.mMousePicker.checkIntersection(mModelProperties.mFactoryMeshes.getMesh("block2")))
	{
		mModelProperties.mFactoryMeshes.getMesh("block2").initMVP(mModelProperties.mProjMatrix,
																  mProgramProperties.mCamera.getViewMatrix(),
																  glm::vec3(10.0f, 10.0f, 10.0f),
																  std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
																  glm::vec3(scale, scale, scale));
		mProgramProperties.mShaderSingleColor.setMatrixUniform4fv("uViewMatrix", mProgramProperties.mCamera.getViewMatrix());
		mProgramProperties.mShaderSingleColor.setMatrixUniform4fv("uModel", mModelProperties.mFactoryMeshes.getMesh("block2").getModelMatrix());
		mProgramProperties.mShaderSingleColor.setMatrixUniform4fv("uMVP", mModelProperties.mFactoryMeshes.getMesh("block2").getMVP());
		mModelProperties.mFactoryMeshes.getMesh("block2").draw();
	}
	if (mProgramProperties.mMousePicker.checkIntersection(mModelProperties.mFactoryMeshes.getMesh("block1")))
	{
		mModelProperties.mFactoryMeshes.getMesh("block1").initMVP(mModelProperties.mProjMatrix, 
																  mProgramProperties.mCamera.getViewMatrix(),
																  glm::vec3(1.0f, 1.0f, 1.0f),
																  std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
																  glm::vec3(scale, scale, scale));
		mProgramProperties.mShaderSingleColor.setMatrixUniform4fv("uViewMatrix", mProgramProperties.mCamera.getViewMatrix());
		mProgramProperties.mShaderSingleColor.setMatrixUniform4fv("uModel", mModelProperties.mFactoryMeshes.getMesh("block1").getModelMatrix());
		mProgramProperties.mShaderSingleColor.setMatrixUniform4fv("uMVP", mModelProperties.mFactoryMeshes.getMesh("block1").getMVP());
		mModelProperties.mFactoryMeshes.getMesh("block1").drawSkybox();
	}	
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glEnable(GL_DEPTH_TEST);
}

void Program::drawNormals()
{
	mProgramProperties.mShaderNormals.bind();
	mModelProperties.mFactoryMeshes.getMesh("block2").initMVP(mModelProperties.mProjMatrix,
															  mProgramProperties.mCamera.getViewMatrix(),
															  glm::vec3(10.0f, 10.0f, 10.0f),
															  std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
															  glm::vec3(5.0f, 5.0f, 5.0f));
	mProgramProperties.mShaderNormals.setUniform3fv("uColor", glm::vec3(1.0f, 0.0f, 1.0f));
	mProgramProperties.mShaderNormals.setMatrixUniform4fv("uModel", mModelProperties.mFactoryMeshes.getMesh("block2").getModelMatrix());
	mProgramProperties.mShaderNormals.setMatrixUniform4fv("uView", mModelProperties.mFactoryMeshes.getMesh("block2").getViewMatrix());
	mProgramProperties.mShaderNormals.setMatrixUniform4fv("uProj", mModelProperties.mProjMatrix);
	mModelProperties.mFactoryMeshes.getMesh("block2").draw();
}

void Program::debugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	}
	std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behavior"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behavior"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	}
	std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	}
	std::cout << std::endl << std::endl;
}