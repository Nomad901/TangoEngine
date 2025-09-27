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

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	mSceneManager.getProgramProperties().mWindowWidth = pWindowWidth;
	mSceneManager.getProgramProperties().mWindowHeight = pWindowHeight;

	mSceneManager.getProgramProperties().mWindow = SDL_CreateWindow("Batch Renderer", pWindowWidth, pWindowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!mSceneManager.getProgramProperties().mWindow)
		std::cout << "Couldnt set the window!\n";
	mSceneManager.getProgramProperties().mContext = SDL_GL_CreateContext(mSceneManager.getProgramProperties().mWindow);
	if (!mSceneManager.getProgramProperties().mContext)
		std::cout << "Couldnt set the context!\n";
	if (!SDL_GL_MakeCurrent(mSceneManager.getProgramProperties().mWindow, mSceneManager.getProgramProperties().mContext))
		std::cout << "Couldnt set the window and the context current!\n";
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		std::cout << "Couldnt load the glad loader!\n";
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	mSceneManager.getProgramProperties().mUI.init(mSceneManager.getProgramProperties().mWindow, 
												  mSceneManager.getProgramProperties().mContext);
}

Program::~Program()
{
	SDL_GL_DestroyContext(mSceneManager.getProgramProperties().mContext);
	SDL_DestroyWindow(mSceneManager.getProgramProperties().mWindow);
	SDL_Quit();
}

void Program::run()
{
	mInitializer.initAll();

	while (mSceneManager.getProgramProperties().mProgIsRunning)
	{
		float beginFrame = SDL_GetTicks();
		
		mRenderer.showFPS();

		mControler.controlAll();
		mRenderer.preDrawScene();
		mRenderer.drawScene();

		float deltaTime = SDL_GetTicks() - beginFrame;
		if (deltaTime < 8)
			SDL_Delay(8 - deltaTime);

		SDL_GL_SwapWindow(mSceneManager.getProgramProperties().mWindow);
	}
}

void Program::drawModels()
{
	mProgramProperties.mShader.bind();
	mProgramProperties.mShader.setUniform3fv("cameraPos", mProgramProperties.mCamera.getPos());
	mMaterialProperties.mMaterial->sendToShaderColored(mProgramProperties.mShader);
	
	//// floor
	mModelProperties.mFactoryMeshes.getMesh("floor").initMVP(mModelProperties.mProjMatrix,
															 mProgramProperties.mCamera.getViewMatrix(),
															 glm::vec3(1.0f, 1.0f, -23.0f),
															 std::make_pair(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
															 glm::vec3(3000.0f, 3000.0f, 1.0f));
	mModelProperties.mFactoryMeshes.getMesh("floor").setUniforms(mProgramProperties.mShader, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//mModelProperties.mFactoryMeshes.getMesh("floor").draw();

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
	
	mMaterialProperties.mMaterial->sendToShader(mProgramProperties.mShader, mModelProperties.mModel[4]->getSlots(), 0, 0);
	mModelProperties.mModel[4]->initMVP(mModelProperties.mProjMatrix,
										mProgramProperties.mCamera.getViewMatrix(),
										glm::vec3(10.0f, 350.0f, -200.0f),
										std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
										glm::vec3(20.0f, 20.0f, 20.0f));
	mModelProperties.mModel[4]->setUniforms(mProgramProperties.mShader, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	mModelProperties.mModel[4]->getFirstTex().bind(GL_TEXTURE_2D, 0);
	mModelProperties.mModel[4]->getSecondTex().bind(GL_TEXTURE_2D, 0);
	mModelProperties.mModel[4]->render();

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
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glEnable(GL_DEPTH_TEST);
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