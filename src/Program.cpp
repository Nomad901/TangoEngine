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

	mProgramProperties.mWindowWidth  = pWindowWidth;
	mProgramProperties.mWindowHeight = pWindowHeight;

	mProgramProperties.mWindow = SDL_CreateWindow("Batch Renderer", pWindowWidth, pWindowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!mProgramProperties.mWindow)
		std::cout << "Couldnt set the window!\n";
	mProgramProperties.mContext = SDL_GL_CreateContext(mProgramProperties.mWindow);
	if (!mProgramProperties.mContext)
		std::cout << "Couldnt set the context!\n";
	if (!SDL_GL_MakeCurrent(mProgramProperties.mWindow, mProgramProperties.mContext))
		std::cout << "Couldnt set the window and the context current!\n";
	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
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

	while (mProgramProperties.mProgIsRunning)
	{
		input();
		preDraw();
		draw();

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
			mProgramProperties.mCamera.mouseMovement(glm::vec2(event.motion.xrel / 5, event.motion.yrel / 5));
	}

	controlScreen();
	controlCamera();
	controlModel();
	controlLight();
}

void Program::preDraw()
{
	float beginFrame = SDL_GetTicks();

	takerCursor();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	mUI.control(*this);

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

	auto posMouseRay = mProgramProperties.mMousePicker.getPosRay();
	std::cout << std::format("Mouse pos: {}-{}-{}\n", posMouseRay.x, posMouseRay.y, posMouseRay.z);

	setLights();
	setLightCube();

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	mProgramProperties.mShader.bind();
	setModels();

	float deltaTime = SDL_GetTicks() - beginFrame;
	if (deltaTime < 8)
		SDL_Delay(8 - deltaTime);
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
	initPrimitives();
	initMeshes();
	initMaterial();
	initModels();
	initLights();
	mProgramProperties.mMousePicker.init(mProgramProperties.mCamera,
										 glm::perspective(glm::radians(45.0f), (float)mProgramProperties.mWindowWidth /
										 (float)mProgramProperties.mWindowHeight, 0.1f, 2000.0f), 
										 { mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight });
	//mProgramProperties.mFBO.init(1280, 720);
}

void Program::initShaders()
{
	mProgramProperties.mShader.init(mProgramProperties.mShader.getResourcePath() + "Shaders/vert.glsl",
									mProgramProperties.mShader.getResourcePath() + "Shaders/frag.glsl");
	mProgramProperties.mShaderSingleColor.init(mProgramProperties.mShader.getResourcePath() + "Shaders/vert.glsl",
											   mProgramProperties.mShader.getResourcePath() + "Shaders/shaderSingleColor.glsl");
}

void Program::initTextures()
{
	mModelProperties.mTextures.reserve(32);
	mModelProperties.mTextures.emplace_back(mProgramProperties.mResourcePath + "CrystalImg.png", "material.textures");
}

void Program::initPrimitives()
{
	// the room
	mModelProperties.mPrimitives.insert_or_assign("museum", std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	
	// floor 
	mModelProperties.mPrimitives.insert_or_assign("floor", std::make_shared<Quad>(mModelProperties.mTextures[0]));

	// light block
	mModelProperties.mPrimitives.insert_or_assign("lightBlock", std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	// character
	mModelProperties.mPrimitives.insert_or_assign("character", std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	// blocks to choose
	mModelProperties.mPrimitives.insert_or_assign("block1", std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	mModelProperties.mPrimitives.insert_or_assign("block2", std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
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
	mModelProperties.mFactoryMeshes.pushMesh("floor", std::make_unique<Mesh>(floor, mModelProperties.mTextures));

	// character
	std::weak_ptr<Primitive> character = mModelProperties.mPrimitives["character"];
	mModelProperties.mFactoryMeshes.pushMesh("character", std::make_unique<Mesh>(character));

	// blocks to choose
	std::weak_ptr<Primitive> block1 = mModelProperties.mPrimitives["block1"];
	mModelProperties.mFactoryMeshes.pushMesh("block1", std::make_unique<Mesh>(block1));
	std::weak_ptr<Primitive> block2 = mModelProperties.mPrimitives["block2"];
	mModelProperties.mFactoryMeshes.pushMesh("block2", std::make_unique<Mesh>(block2));
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
}

void Program::initLights()
{
	mLightProperties.mLightManager.pushLight("pointLight1", std::make_unique<PointLight>(glm::vec3( 12.0f,  91.0f, -302.0f), 1.0f, 0.045f, 0.075f));
	mLightProperties.mLightManager.pushLight("pointLight2", std::make_unique<PointLight>(glm::vec3(-109.0f, 91.0f, -302.0f), 1.0f, 0.045f, 0.075f));
	mLightProperties.mLightManager.pushLight("pointLight3", std::make_unique<PointLight>(glm::vec3( 5.0f,   94.0f, -724.0f), 1.0f, 0.045f, 0.075f));
	mLightProperties.mLightManager.pushLight("pointLight4", std::make_unique<PointLight>(glm::vec3(-117.0f, 94.0f, -724.0f), 1.0f, 0.045f, 0.075f));
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
	float speedCamera = 0.5f;
	if (mProgramProperties.mKeyCodes[SDLK_LSHIFT])
		speedCamera = 2.0f;
	if (mProgramProperties.mKeyCodes[SDLK_W])
		mProgramProperties.mCamera.moveForward(speedCamera);
	if (mProgramProperties.mKeyCodes[SDLK_S])
		mProgramProperties.mCamera.moveBackward(speedCamera);
	if (mProgramProperties.mKeyCodes[SDLK_A])
		mProgramProperties.mCamera.moveLeft(speedCamera);
	if (mProgramProperties.mKeyCodes[SDLK_D])
		mProgramProperties.mCamera.moveRight(speedCamera);
	if (mProgramProperties.mKeyCodes[SDLK_LCTRL])
		mProgramProperties.mCamera.moveDown(speedCamera);
	if (mProgramProperties.mKeyCodes[SDLK_SPACE])
		mProgramProperties.mCamera.moveUp(speedCamera);
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
 	//mProgramProperties.mShader.bind();
	//mModelProperties.mFactoryMeshes.getMesh("Room").initMVP(mProgramProperties.mWindowWidth,
	//														mProgramProperties.mWindowHeight,
	//														mProgramProperties.mCamera.getViewMatrix(),
	//														glm::vec3(2.0f, 2.0f, -55.0f), std::make_pair(-90.0f, glm::vec3(1.0f, 0.0, 0.0f)),
	//														glm::vec3(100.0f, 100.0f, 100.0f));
	//mModelProperties.mFactoryMeshes.getMesh("Room").setUniforms(mProgramProperties.mCamera.getPos(), mProgramProperties.mCamera.getViewMatrix(), 
	//															 glm::vec4(0.5f, 0.5f, 0.5f, 0.5f),
	//															 mProgramProperties.mShader, *mMaterialProperties.mMaterial, true);
	//mLightProperties.mPointLight.setPosLight(mLightProperties.mPosLight);
	//mModelProperties.mFactoryMeshes.getMesh("Room").draw();

	glStencilMask(0x00);
	// floor
	mProgramProperties.mShader.bind();
	mModelProperties.mFactoryMeshes.getMesh("floor").initMVP(mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight,
															  mProgramProperties.mCamera.getViewMatrix(),
															  glm::vec3(1.0f, 1.0f, -23.0f),
															  std::make_pair(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
															  glm::vec3(3000.0f, 3000.0f, 1.0f));
	mProgramProperties.mShader.setMatrixUniform4fv("uModel", mModelProperties.mFactoryMeshes.getMesh("floor").getModelMatrix());
	mProgramProperties.mShader.setMatrixUniform4fv("uMVP", mModelProperties.mFactoryMeshes.getMesh("floor").getMVP(false));
	mMaterialProperties.mMaterial->sendToShader(mProgramProperties.mShader, false);
	mModelProperties.mFactoryMeshes.getMesh("floor").draw();

	// museum
	//mProgramProperties.mFBO.bindWrite();
	mModelProperties.mModel[0]->initMVP(mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight,
										mProgramProperties.mCamera.getViewMatrix(),
										glm::vec3(1.0f, -34.5f, -1147.0f),
										std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
										glm::vec3(1.0f, 1.0f, 1.0f));
	mProgramProperties.mShader.setUniform3fv("uObjectColor", glm::vec3(1.0f, 1.0f, 1.0f));
	mProgramProperties.mShader.setUniform3fv("cameraPos", mProgramProperties.mCamera.getPos());
	mProgramProperties.mShader.setMatrixUniform4fv("uViewMatrix", mProgramProperties.mCamera.getViewMatrix());
	mProgramProperties.mShader.setMatrixUniform4fv("uModel", mModelProperties.mModel[0]->getModelMatrix());
	mProgramProperties.mShader.setMatrixUniform4fv("uMVP", mModelProperties.mModel[0]->getMVP(false));
	mMaterialProperties.mMaterial->sendToShader(mProgramProperties.mShader, true);
	mModelProperties.mModel[0]->render();
	
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	mModelProperties.mFactoryMeshes.getMesh("block1").initMVP(mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight,
															  mProgramProperties.mCamera.getViewMatrix(),
															  glm::vec3(1.0f, 1.0f, 1.0f),
															  std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
															  glm::vec3(5.0f, 5.0f, 5.0f));
	mProgramProperties.mShader.setUniform3fv("uObjectColor", glm::vec3(1.0f, 1.0f, 1.0f));
	mProgramProperties.mShader.setUniform3fv("cameraPos", mProgramProperties.mCamera.getPos());
	mProgramProperties.mShader.setMatrixUniform4fv("uViewMatrix", mProgramProperties.mCamera.getViewMatrix());
	mProgramProperties.mShader.setMatrixUniform4fv("uModel", mModelProperties.mFactoryMeshes.getMesh("block1").getModelMatrix());
	mProgramProperties.mShader.setMatrixUniform4fv("uMVP", mModelProperties.mFactoryMeshes.getMesh("block1").getMVP(false));
	mMaterialProperties.mMaterial->sendToShader(mProgramProperties.mShader, true);
	mModelProperties.mFactoryMeshes.getMesh("block1").draw();

	mModelProperties.mFactoryMeshes.getMesh("block2").initMVP(mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight,
															  mProgramProperties.mCamera.getViewMatrix(),
															  glm::vec3(10.0f, 10.0f, 10.0f),
															  std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
															  glm::vec3(5.0f, 5.0f, 5.0f));
	mProgramProperties.mShader.setUniform3fv("uObjectColor", glm::vec3(1.0f, 1.0f, 1.0f));
	mProgramProperties.mShader.setUniform3fv("cameraPos", mProgramProperties.mCamera.getPos());
	mProgramProperties.mShader.setMatrixUniform4fv("uViewMatrix", mProgramProperties.mCamera.getViewMatrix());
	mProgramProperties.mShader.setMatrixUniform4fv("uModel", mModelProperties.mFactoryMeshes.getMesh("block2").getModelMatrix());
	mProgramProperties.mShader.setMatrixUniform4fv("uMVP", mModelProperties.mFactoryMeshes.getMesh("block2").getMVP(false));
	mMaterialProperties.mMaterial->sendToShader(mProgramProperties.mShader, true);
	mModelProperties.mFactoryMeshes.getMesh("block2").draw();


	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	mProgramProperties.mShader.unbind();
	mProgramProperties.mShaderSingleColor.bind();
	float scale = 5.1f;
	mModelProperties.mFactoryMeshes.getMesh("block1").initMVP(mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight,
										mProgramProperties.mCamera.getViewMatrix(),
										glm::vec3(1.0f, 1.0f, 1.0f),
										std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
										glm::vec3(scale, scale, scale));
	mProgramProperties.mShaderSingleColor.setMatrixUniform4fv("uViewMatrix", mProgramProperties.mCamera.getViewMatrix());
	mProgramProperties.mShaderSingleColor.setMatrixUniform4fv("uModel", mModelProperties.mFactoryMeshes.getMesh("block1").getModelMatrix());
	mProgramProperties.mShaderSingleColor.setMatrixUniform4fv("uMVP", mModelProperties.mFactoryMeshes.getMesh("block1").getMVP(false));
	mModelProperties.mFactoryMeshes.getMesh("block1").draw();
	
	mModelProperties.mFactoryMeshes.getMesh("block2").initMVP(mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight,
															  mProgramProperties.mCamera.getViewMatrix(),
															  glm::vec3(10.0f, 10.0f, 10.0f),
															  std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
															  glm::vec3(scale, scale, scale));
	mProgramProperties.mShaderSingleColor.setMatrixUniform4fv("uViewMatrix", mProgramProperties.mCamera.getViewMatrix());
	mProgramProperties.mShaderSingleColor.setMatrixUniform4fv("uModel", mModelProperties.mFactoryMeshes.getMesh("block2").getModelMatrix());
	mProgramProperties.mShaderSingleColor.setMatrixUniform4fv("uMVP", mModelProperties.mFactoryMeshes.getMesh("block2").getMVP(false));
	mModelProperties.mFactoryMeshes.getMesh("block2").draw();
	

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glEnable(GL_DEPTH_TEST);
	mProgramProperties.mShaderSingleColor.unbind();
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