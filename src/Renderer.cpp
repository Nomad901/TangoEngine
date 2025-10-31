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

	//
	// gbuffer and terrain 
	//
	auto gbufferRef = &mSceneManager->mProgramProperties.mGBuffer;

	geometryPass(gbufferRef);
	beginLightPass(gbufferRef);
	pointLightPass(gbufferRef);
	directionalLightPass(gbufferRef);

	//
	// skybox 
	//
	mSceneManager->mProgramProperties.mSkybox->render(mSceneManager->mProgramProperties.mShaders["skyboxShader"]);
	//
	// fps 
	//
	showFPS();
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::showFPS()
{
	static float prevTime = 0.0f;
	static float currTime = 0.0f;
	static float timeDiff = 0.0f;
	static uint32_t counter = 0;
	static uint32_t screenWidth = mSceneManager->getProgramProperties().mWindowWidth;
	static uint32_t screenHeight = mSceneManager->getProgramProperties().mWindowHeight;
	static std::string fpsText = "";
	
	currTime = SDL_GetTicks();
	timeDiff = currTime - prevTime;
	counter++;
	if (timeDiff >= 1.0f / 100.0f)
	{
		std::string fps = std::to_string(static_cast<int32_t>((1.0f / timeDiff) * counter * 1000));
		//std::string ms = std::to_string((timeDiff / counter) * 1000);
		fpsText = "FPS: " + fps;
		prevTime = currTime;
		counter = 0;
	}
	mSceneManager->mProgramProperties.mFontSystem.renderText(screenWidth, screenHeight, fpsText, mSceneManager->mProgramProperties.mPosFont, 
																								 mSceneManager->mProgramProperties.mScaleFont, 
																								 mSceneManager->mProgramProperties.mColorFont);
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
	//glEnable(GL_MULTISAMPLE);

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

void Renderer::geometryPass(GBuffer* pBuffer)
{
	pBuffer->bindForWriting();

	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	mSceneManager->mModelProperties.mTerrain->render(&mSceneManager->getProgramProperties().mThirdPersonCam, mSceneManager->mModelProperties.mProjMatrix);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
}

void Renderer::beginLightPass(GBuffer* pBuffer)
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	pBuffer->bindForReading();
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::pointLightPass(GBuffer* pBuffer)
{
	mSceneManager->mProgramProperties.mShaders.getShader("pointLight").bind();
	mSceneManager->mProgramProperties.mShaders.getShader("pointLight").setUniform3fv("uViewWorldPos", 
																					 mSceneManager->getProgramProperties().mThirdPersonCam.getPos());
	static std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>> lights = 
		   std::make_pair(mSceneManager->mLightProperties.lightPositions, 
						  mSceneManager->mLightProperties.lightColors);

	for (size_t i = 0; i < lights.first.size(); ++i)
	{

	}
	
}

void Renderer::directionalLightPass(GBuffer* pBuffer)
{

}

void Renderer::spotLightPass(GBuffer* pBuffer)
{

}

float Renderer::calcPointLightSphere(const glm::vec3& pLightPos)
{

	return 0.0f;
}

void Renderer::renderLightSphere()
{

}


//void Renderer::lightPass(GBuffer* pBuffer)
//{
//	pBuffer->unbind();
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	pBuffer->bindForReading();
//	
//	uint32_t windowWidth = mSceneManager->mProgramProperties.mWindowWidth;
//	uint32_t windowHeight = mSceneManager->mProgramProperties.mWindowHeight;
//	uint32_t halfWindowWidth = windowWidth / 2;
//	uint32_t halfWindowHeight = windowHeight / 2;
//	
//	//pBuffer->setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_POSITION);
//	//glBlitFramebuffer(halfWindowWidth, halfWindowHeight, windowWidth, windowHeight, 
//	//				  halfWindowWidth, halfWindowHeight, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
//
//	//pBuffer->setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_DIFFUSE);
//	//glBlitFramebuffer(0, halfWindowHeight, halfWindowWidth, windowHeight, 
//	//				  0, halfWindowHeight, halfWindowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
//	
//	//pBuffer->setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_NORMAL);
//	//glBlitFramebuffer(0, 0, halfWindowWidth, halfWindowHeight, 
//	//				  0, 0, halfWindowWidth, halfWindowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
//
//	//pBuffer->setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_NORMAL);
//	//glBlitFramebuffer(0, 0, windowWidth, windowHeight,
//	//				  0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
//
//	//pBuffer->setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXCOORD);
//	//glBlitFramebuffer(halfWindowWidth, 0, windowWidth, halfWindowHeight, 
//	//				  halfWindowWidth, 0, windowWidth, halfWindowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
//
//	if (mSceneManager->getProgramProperties().currentTexture == 0)
//	{
//		pBuffer->setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_POSITION);
//		glBlitFramebuffer(0, 0, windowWidth, windowHeight,
//						  0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
//	}
//	else if (mSceneManager->getProgramProperties().currentTexture == 2)
//	{
//		pBuffer->setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_DIFFUSE);
//		glBlitFramebuffer(0, 0, windowWidth, windowHeight,
//						  0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
//	}
//	else if (mSceneManager->getProgramProperties().currentTexture == 3)
//	{
//		pBuffer->setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_NORMAL);
//		glBlitFramebuffer(0, 0, windowWidth, windowHeight,
//						  0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
//	}
//	else
//	{
//		pBuffer->setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXCOORD);
//		glBlitFramebuffer(0, 0, windowWidth, windowHeight,
//						  0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
//	}
//
//	pBuffer->unbindForReading();
//}