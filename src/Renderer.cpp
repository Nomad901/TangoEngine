#include "Renderer.h"
#include "Controler.h"

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

void Renderer::drawScene(Controler* pControler)
{
	ImGui::EndFrame();
	mSceneManager->getProgramProperties().mWaterTiles[0].setWaterPos(mSceneManager->getProgramProperties().mWaterPos);
	mSceneManager->getProgramProperties().mWaterTiles[0].setTileSize(mSceneManager->getProgramProperties().mWaterScale);

	float waterHeight = mSceneManager->getProgramProperties().mWaterTiles[0].getWaterHeight();

	glEnable(GL_CLIP_DISTANCE0);

	mSceneManager->getProgramProperties().mWaterFBO->getReflectionFBO().start();
	mSceneManager->getModelProperties().mPlaneTerrainHeight = glm::vec4(0.0f, 1.0f, 0.0f, -waterHeight + 1.0f);
	auto camera = &mSceneManager->getProgramProperties().mThirdPersonCam;
	float distance = (camera->getPos().y - mSceneManager->getProgramProperties().mWaterTiles[0].getWaterPos().y);
	camera->setPos(glm::vec3(camera->getPos().x, camera->getPos().y - distance, camera->getPos().z));
	camera->setPitch(-camera->getPitch());
	camera->update(pControler->getEvents(), pControler->getPlayer().getPos());
	drawSceneTMP();
	camera->setPos(glm::vec3(camera->getPos().x, camera->getPos().y + distance, camera->getPos().z));
	camera->setPitch(-camera->getPitch());
	camera->update(pControler->getEvents(), pControler->getPlayer().getPos());
	mSceneManager->getProgramProperties().mWaterFBO->getReflectionFBO().stop();
	glViewport(0, 0, mSceneManager->getProgramProperties().mWindowWidth,
					 mSceneManager->getProgramProperties().mWindowHeight);
	
	mSceneManager->getProgramProperties().mWaterFBO->getRefractionFBO().start();
	mSceneManager->getModelProperties().mPlaneTerrainHeight = glm::vec4(0.0f, -1.0f, 0.0f, waterHeight + 0.1f);
	drawSceneTMP();
	mSceneManager->getProgramProperties().mWaterFBO->getRefractionFBO().stop();
	glViewport(0, 0, mSceneManager->getProgramProperties().mWindowWidth,
				     mSceneManager->getProgramProperties().mWindowHeight);
	
	glDisable(GL_CLIP_DISTANCE0);
	
	mSceneManager->getModelProperties().mPlaneTerrainHeight = glm::vec4(0.0f, 1.0f, 0.0f, 10000.0f);
	drawSceneTMP();
	mSceneManager->getProgramProperties().mWaterRenderer.render(mSceneManager->getProgramProperties().mWaterTiles,
																mSceneManager->getProgramProperties().mThirdPersonCam,
																mSceneManager->getModelProperties().mProjMatrix,
															   *mSceneManager->getProgramProperties().mWaterFBO.get(),
																mSceneManager->getLightProperties().mSun,
																mSceneManager->getProgramProperties().NEAR_PLANE, 
																mSceneManager->getProgramProperties().FAR_PLANE,
																mSceneManager->getProgramProperties().mWaterColor);

	//mSceneManager->getProgramProperties().mWaterFBO->getReflectionFBO().render();
	//mSceneManager->getProgramProperties().mWaterFBO->getRefractionFBO().render();
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::drawSceneTMP()
{
	glCullFace(GL_FRONT);
	mSceneManager->mModelProperties.mTerrain->render(&mSceneManager->getProgramProperties().mThirdPersonCam, 
													  mSceneManager->mModelProperties.mProjMatrix,
													  mSceneManager->getModelProperties().mPlaneTerrainHeight);
	mSceneManager->getModelProperties().mModelManager.getModel("WaterTerrain").render();
	glCullFace(GL_BACK);

	//
	// Skybox
	//
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	mSceneManager->mProgramProperties.mSkybox->render(mSceneManager->mProgramProperties.mShaders["skyboxShader"]);
	//glDepthFunc(GL_LESS);

	// 
	// Light cubes and fps
	//
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	renderCubeLights();

	Camera& cameraForChar = mSceneManager->getProgramProperties().mThirdPersonCam;
	glm::mat4& projMat = mSceneManager->getModelProperties().mProjMatrix;
	Timer& time = mSceneManager->getProgramProperties().mTimer;

	auto bobAnim = mSceneManager->getModelProperties().mAnimatorManager.getAnimator("bobAnim");
	
	bobAnim->getModelTransformation().setLocalPosition(mSceneManager->getModelProperties().mCharPos);
	bobAnim->getModelTransformation().setLocalRotation(mSceneManager->getModelProperties().mCharRotation);
	bobAnim->getModelTransformation().setLocalScale(mSceneManager->getModelProperties().mCharScale);

	bobAnim->update(cameraForChar, projMat, time);
	bobAnim->render();
}

void Renderer::renderCubeLights()
{
	static uint32_t cubeVAO = 0;
	static uint32_t cubeVBO = 0;
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			 // bottom face
			 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 // top face
			 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	auto singleColorShader = &mSceneManager->getProgramProperties().mShaders.getShader("singleColorShader");
	singleColorShader->bind();
	uint32_t index = 0;
	for (auto& i : mSceneManager->getLightProperties().lightPositions)
	{
		glm::mat4 MVP = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, i);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		MVP = mSceneManager->getModelProperties().mProjMatrix * mSceneManager->getProgramProperties().mThirdPersonCam.getViewMatrix() * model;
		singleColorShader->setMatrixUniform4fv("uMVP", MVP);
		singleColorShader->setUniform3fv("uColor", mSceneManager->getLightProperties().lightColors[index]);
		
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
		index++;
	}
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