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
	glViewport(0, 0, mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight);
	if (mProgramProperties.mWhiteScreen)
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	else
		glClearColor(0.20f, 0.20f, 0.20f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setLights();
	setLightCube();
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
	initShader();
	initTextures();
	initPrimitives();
	initMeshes();
	initMaterial();
	initModels();
	initLights();
}

void Program::initShader()
{
	mProgramProperties.mShader.init(mProgramProperties.mResourcePath + "vert.glsl", 
									mProgramProperties.mResourcePath + "frag.glsl");
	mProgramProperties.mLightShader.init(mProgramProperties.mResourcePath + "vert.glsl", mProgramProperties.mResourcePath + "light.glsl");
}

void Program::initTextures()
{
	mModelProperties.mTextures.reserve(32);
	mModelProperties.mTextures.emplace_back(mProgramProperties.mResourcePath + "dcq.png", "material.textures");
	
}

void Program::initPrimitives()
{
	// the room
	mModelProperties.mPrimitives.insert_or_assign("museum", std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	
	// light block
	mModelProperties.mPrimitives.insert_or_assign("lightBlock", std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

}

void Program::initMeshes()
{
	// the room
	std::weak_ptr<Primitive> museum = mModelProperties.mPrimitives["museum"];
	mModelProperties.mFactoryMeshes.pushMesh("Room", std::make_unique<Mesh>(museum));

	// light block
	std::weak_ptr<Primitive> lightBlock = mModelProperties.mPrimitives["lightBlock"];
	mModelProperties.mFactoryMeshes.pushMesh("lightBlock", std::make_unique<Mesh>(lightBlock));

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
														      mProgramProperties.mResourcePath + "museum.obj", 
															  std::vector<Texture2>()));
}

void Program::initLights()
{
	mLightProperties.mPointLight.init(mLightProperties.mPosLight, false);

	PointLight pointLight0(mLightProperties.mPointLight.getPosLight(), false);
	pointLight0.setColor(mLightProperties.mLightColor);
	pointLight0.setIntensity(0.7f);

	mLightProperties.mStrgLights.reserve(3);
	mLightProperties.mStrgLights.push_back(std::move(pointLight0));
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
	//mLightProperties.mStrgLights[0].setPosLight(mLightProperties.mPosLight);
	mLightProperties.mStrgLights[0].setPosLight(mProgramProperties.mCamera.getPos());
	mLightProperties.mStrgLights[0].setColor(mLightProperties.mLightColor);

	setMaterials();

	mProgramProperties.mShader.bind();
	mMaterialProperties.mMaterial->sendToShader(mProgramProperties.mShader, true);
	mLightProperties.mStrgLights[0].sendToShaderArray(mProgramProperties.mShader, 0);
	mProgramProperties.mShader.setUniform3fv("point_light[0].ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	mProgramProperties.mShader.setUniform3fv("point_light[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	mProgramProperties.mShader.setUniform3fv("point_light[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	
	mProgramProperties.mShader.setUniform1f("point_light[0].constant", 1.0f);
	mProgramProperties.mShader.setUniform1f("point_light[0].linear", 0.09f);
	mProgramProperties.mShader.setUniform1f("point_light[0].quadratic", 0.032f);
	
	mProgramProperties.mShader.setUniform1i("uNumberOfLights", mLightProperties.mStrgLights.size());
	mProgramProperties.mShader.setUniform3fv("point_light[0].direction", mProgramProperties.mCamera.getDirection());
	mProgramProperties.mShader.setUniform1f("point_light[0].cutOff", glm::cos(glm::radians(12.5f)));
	mProgramProperties.mShader.setUniform1f("point_light[0].outerCutOff", glm::cos(glm::radians(17.5f)));
}

void Program::setLightCube()
{
	//mProgramProperties.mLightShader.bind();
	//mModelProperties.mModel[0]->initMVP(mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight, mProgramProperties.mCamera.getViewMatrix(),
	//									mLightProperties.mStrgLights[0].getPosLight(),
	//									std::make_pair(45.0f, glm::vec3(0.0f, 0.0f, 0.0f)), 
	//									glm::vec3(1.0f, 1.0f, 1.0f));
	//mModelProperties.mModel[0]->setPos(mLightProperties.mStrgLights[0].getPosLight(), true);
	//mProgramProperties.mLightShader.setMatrixUniform4fv("uModel", mModelProperties.mModel[0]->getModelMatrix());
	//mProgramProperties.mLightShader.setMatrixUniform4fv("uMVP", mModelProperties.mModel[0]->getMVP(false));
	//mProgramProperties.mLightShader.setUniform3fv("uColor", mLightProperties.mStrgLights[0].getColor());
	//mModelProperties.mModel[0]->render();

	mProgramProperties.mLightShader.bind();
	mModelProperties.mFactoryMeshes.getMesh("lightBlock").initMVP(mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight, 
																  mProgramProperties.mCamera.getViewMatrix(),
																  mLightProperties.mStrgLights[0].getPosLight(), 
																  std::make_pair(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
																  glm::vec3(1.0f, 1.0f, 1.0f));
	mProgramProperties.mLightShader.setMatrixUniform4fv("uModel", mModelProperties.mFactoryMeshes.getMesh("lightBlock").getModelMatrix());
	mProgramProperties.mLightShader.setMatrixUniform4fv("uMVP", mModelProperties.mFactoryMeshes.getMesh("lightBlock").getMVP(false));
	mProgramProperties.mLightShader.setUniform3fv("uColor", mLightProperties.mStrgLights[0].getColor());
	//mModelProperties.mFactoryMeshes.getMesh("lightBlock").draw();
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

	mProgramProperties.mShader.bind();
	mModelProperties.mModel[0]->initMVP(mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight,
										mProgramProperties.mCamera.getViewMatrix(),
										glm::vec3(1.0f, 1.0f, -1000.0f),
										std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
										glm::vec3(1.0f, 1.0f, 1.0f));
	mProgramProperties.mShader.setUniform3fv("uObjectColor", glm::vec3(1.0f, 1.0f, 1.0f));
	mProgramProperties.mShader.setUniform3fv("cameraPos", mProgramProperties.mCamera.getPos());
	mProgramProperties.mShader.setMatrixUniform4fv("uViewMatrix", mProgramProperties.mCamera.getViewMatrix());
	mProgramProperties.mShader.setMatrixUniform4fv("uModel", mModelProperties.mModel[0]->getModelMatrix());
	mProgramProperties.mShader.setMatrixUniform4fv("uMVP", mModelProperties.mModel[0]->getMVP(false));
	mMaterialProperties.mMaterial->sendToShader(mProgramProperties.mShader, true);
	mModelProperties.mModel[0]->render();
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
