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

	mSceneManager->getProgramProperties().mGBuffer.bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mSceneManager->mModelProperties.mTerrain->render(&mSceneManager->getProgramProperties().mThirdPersonCam, mSceneManager->mModelProperties.mProjMatrix);
	mSceneManager->getProgramProperties().mGBuffer.unbind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mSceneManager->getProgramProperties().mGBuffer.getGPosBuffer());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mSceneManager->getProgramProperties().mGBuffer.getGNormalBuffer());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mSceneManager->getProgramProperties().mGBuffer.getGColorSpecBuffer());

	auto shader = &mSceneManager->getProgramProperties().mShaders["DeferredLight"];
	shader->bind();
	shader->setUniform1i("gPos", 0);
	shader->setUniform1i("gNormals", 1);
	shader->setUniform1i("gSpec", 2);
	shader->setUniform3fv("uLight.position", mSceneManager->getProgramProperties().mThirdPersonCam.getPos());
	shader->setUniform3fv("uLight.color", glm::vec3(1.0f, 1.0f, 1.0f));

	const float linear = 0.7f;
	const float quadratic = 1.8f;
	shader->setUniform1f("uLight.linear", linear);
	shader->setUniform1f("uLight.quadratic", quadratic);
	const float constant = 1.0f;
	const float maxBrightness = std::fmaxf(std::fmaxf(0.0f, 1.0f), 1.0f);
	const float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
	shader->setUniform1f("uLight.radius", radius);
	shader->setUniform3fv("uViewPos", mSceneManager->getProgramProperties().mThirdPersonCam.getPos());

	static uint32_t quadVAO = 0;
	static uint32_t quadVBO;
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	uint32_t screenWidth = mSceneManager->getProgramProperties().mWindowWidth;
	uint32_t screenHeight = mSceneManager->getProgramProperties().mWindowHeight;
	mSceneManager->getProgramProperties().mGBuffer.bind();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	mSceneManager->getProgramProperties().mGBuffer.unbind();

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
