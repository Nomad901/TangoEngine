#include "Renderer.h"

Renderer::Renderer(SceneManager* pSceneManager)
{
	mSceneManager = pSceneManager;
}

void Renderer::preDrawScene()
{
	takeCursor();
	setImGui();
	setGLproperties();

	mSceneManager->mProgramProperties.mUBO.appendData(0, mSceneManager->mModelProperties.mProjMatrix);
	mSceneManager->mProgramProperties.mUBO.appendData(sizeof(glm::mat4), 
													  mSceneManager->mProgramProperties.mCamera.getViewMatrix());
}

void Renderer::drawScene()
{
	ImGui::EndFrame();

	// terrain
	glm::vec3 camPos = mSceneManager->getProgramProperties().mCamera.getPos();
	mSceneManager->mModelProperties.mTerrain->render(mSceneManager->getProgramProperties().mViewMatrix, 
													 mSceneManager->mModelProperties.mProjMatrix,
													 camPos);

	// crosshair
	//mSceneManager->mProgramProperties.mCrosshair->render(mSceneManager->mProgramProperties.mWindowWidth, 
	//													 mSceneManager->mProgramProperties.mWindowHeight);

	// main shader part
	//mSceneManager->mProgramProperties.mShaders["mainShader"].bind();
	//mSceneManager->mModelProperties.mModelManager.getModel("museum").render(mSceneManager->mProgramProperties.mShaders["mainShader"]);
	//mSceneManager->mModelProperties.mModelManager["lampPost1"].render(mSceneManager->mProgramProperties.mShaders["mainShader"]);
	//mSceneManager->mModelProperties.mModelManager["lampPost2"].render(mSceneManager->mProgramProperties.mShaders["mainShader"]);

	// skybox 
	mSceneManager->mProgramProperties.mSkybox->render(mSceneManager->mProgramProperties.mShaders["skyboxShader"]);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::showFPS()
{
	static float prevTime = 0.0f;
	static float currTime = 0.0f;
	static float timeDiff = 0.0f;
	static uint32_t counter = 0;

	currTime = SDL_GetTicks();
	timeDiff = currTime - prevTime;
	counter++;
	if (timeDiff >= 1.0f / 100.0f)
	{
		std::string fps = std::to_string((1.0f / timeDiff) * counter * 1000);
		std::string ms = std::to_string((timeDiff / counter) * 1000);
		std::string newTitle = std::format("Museum of lights. FPS: {} | MS: {}", fps, ms);
		SDL_SetWindowTitle(mSceneManager->mProgramProperties.mWindow, newTitle.c_str());
		prevTime = currTime;
		counter = 0;
	}
}

void Renderer::takeCursor()
{
	if (mSceneManager->mProgramProperties.mTakeCursor)
	{
		SDL_WarpMouseInWindow(mSceneManager->mProgramProperties.mWindow,
			mSceneManager->mProgramProperties.mWindowWidth / 2,
			mSceneManager->mProgramProperties.mWindowHeight / 2);
		SDL_SetWindowRelativeMouseMode(mSceneManager->mProgramProperties.mWindow, true);
	}
	else
		SDL_SetWindowRelativeMouseMode(mSceneManager->mProgramProperties.mWindow, false);
}

void Renderer::setImGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	mSceneManager->mProgramProperties.mUI.control(*mSceneManager);
}

void Renderer::setGLproperties()
{
	// BLEND
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// MSAA
	glEnable(GL_MULTISAMPLE);

	// OPTIMIZATION
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// WIREFRAME MODE
	if (mSceneManager->mProgramProperties.mWireFrameMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// DEPTH TEST
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glViewport(0, 0, mSceneManager->mProgramProperties.mWindowWidth,
		mSceneManager->mProgramProperties.mWindowHeight);
	if (mSceneManager->mProgramProperties.mWhiteScreen)
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	else
		glClearColor(0.20f, 0.20f, 0.20f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
