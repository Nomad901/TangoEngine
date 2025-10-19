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
	
	auto fboRef = mSceneManager->getProgramProperties().mFBO.get();

	fboRef->bind();
	glm::vec2 size = fboRef->getSize();
	glViewport(0, 0, size.x, size.y);
	fboRef->setClearColors(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
	fboRef->clearColor();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	uint32_t winWidth  = mSceneManager->getProgramProperties().mWindowWidth;
	uint32_t winHeight = mSceneManager->getProgramProperties().mWindowHeight;

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, winWidth / 2, winHeight);
	fboRef->getShader().bind();
	fboRef->getShader().setUniform1i("uTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fboRef->getTexture().getID());
	fboRef->getScreenQuad().render();
	mSceneManager->mModelProperties.mTerrain->render(&mSceneManager->getProgramProperties().mThirdPersonCam, mSceneManager->mModelProperties.mProjMatrix);

	glViewport(winWidth / 2, 0, winWidth / 2, winHeight);

	mSceneManager->mModelProperties.mTerrain->render(&mSceneManager->getProgramProperties().mThirdPersonCam, mSceneManager->mModelProperties.mProjMatrix);

	glViewport(0, 0, winWidth, winHeight);

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
