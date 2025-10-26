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

	mSceneManager.getProgramProperties().mWindow = SDL_CreateWindow("TangoEngine", pWindowWidth, pWindowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
	mInitializer.init(true);
	mControler = std::make_unique<Controler>(&mSceneManager);

	std::vector<Mesh*> meshes;
	meshes.reserve(mSceneManager.getModelProperties().mFactoryMeshes.getStorageMeshes().size() + 
				   mSceneManager.getModelProperties().mModelManager.getStorageModels().size());

	while (mSceneManager.getProgramProperties().mProgIsRunning)
	{
		float beginFrame = SDL_GetTicks();
		static uint32_t lastTime = SDL_GetTicks();
		uint32_t currentTime = SDL_GetTicks();

		float physicsDeltaTime = (currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;

		meshes.clear();
		for (auto& [key, value] : mSceneManager.getModelProperties().mFactoryMeshes.getStorageMeshes())
		{
			meshes.push_back(value.get());
		}
		for (auto& [key, value] : mSceneManager.getModelProperties().mModelManager.getStorageModels())
		{
			for (auto& mesh : value.get()->getMeshes())
			{
				meshes.push_back(mesh.get());
			}
		}
		//meshes.push_back(&mSceneManager.getModelProperties().mTerrain.get()->getMesh());

		mControler->controlAll(physicsDeltaTime);
		mControler->getPlayer().update(mSceneManager.getModelProperties().mProjMatrix, physicsDeltaTime, meshes, 
									   mSceneManager.getModelProperties().mTerrain.get());

		mSceneManager.setAll();
		mRenderer.preDrawScene();
		mRenderer.drawScene();
		mControler->getPlayer().renderCharacter(mSceneManager.getProgramProperties().mShaders["mainShader"]);
		
		float deltaTime = SDL_GetTicks() - beginFrame;
		if (deltaTime < 8)
			SDL_Delay(8 - deltaTime);
		Utils::getInstance().updateDeltaTime(deltaTime); // TODO: maybe i need to recompute the delta time, cuz ive set the delay on the program;

		SDL_GL_SwapWindow(mSceneManager.getProgramProperties().mWindow);
	}
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