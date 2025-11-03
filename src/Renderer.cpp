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
	
	//
	// geometry pass
	//
	geometryPass(gbufferRef);

	//
	// light pass
	//
	beginLightPass(gbufferRef);
	pointLightPass();
	directionalLightPass();
	
	//
	// copying the generated framebuffer into the main one;
	//
	uint32_t screenWidth  = mSceneManager->getProgramProperties().mWindowWidth;
	uint32_t screenHeight = mSceneManager->getProgramProperties().mWindowHeight;
	gbufferRef->unbind();
	gbufferRef->bindForReading();
	glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	gbufferRef->unbindForReading();

	//
	// Skybox
	//
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_LEQUAL);
	glDisable(GL_BLEND);

	mSceneManager->mProgramProperties.mSkybox->render(mSceneManager->mProgramProperties.mShaders["skyboxShader"]);

	// 
	// Light cubes and fps
	//
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	renderCubeLights();
	showFPS();
	glBlendFunc(GL_ONE, GL_ONE);

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

	pBuffer->bindForReading(mSceneManager->getProgramProperties().mShaders.getShader("pointLight"),
							{ "uPositionMap", "uColorMap", "uNormalMap" });

	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::pointLightPass()
{
	auto pointLightShader = &mSceneManager->mProgramProperties.mShaders.getShader("pointLight");
	pointLightShader->setUniform3fv("uViewWorldPos", mSceneManager->getProgramProperties().mThirdPersonCam.getPos());
	pointLightShader->setUniform1i("uNumberLightsToProcess", mSceneManager->mLightProperties.lightPositions.size());
	pointLightShader->setUniform2fv("uScreenSize", glm::vec2(mSceneManager->getProgramProperties().mWindowWidth, 
															 mSceneManager->getProgramProperties().mWindowHeight));

	static std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>> lights = 
		   std::make_pair(mSceneManager->mLightProperties.lightPositions, 
						  mSceneManager->mLightProperties.lightColors);
	pointLightShader->setUniform1i("uNumberLightsToProcess", static_cast<int32_t>(lights.first.size()));

	Transform transform;
	float ambientIntensity = 0.1f;   
	float diffuseIntensity = 1.0f;   
	float constant = 1.0f;           
	float linear = 0.09f;            
	float exp = 0.032f;              
	for (size_t i = 0; i < lights.first.size(); ++i)
	{
		pointLightShader->setUniform3fv("uPointLight[" + std::to_string(i) + "].mBaseLight.mColor", lights.second[i]);
		pointLightShader->setUniform1f("uPointLight[" + std::to_string(i) + "].mBaseLight.mAmbientIntensity", ambientIntensity);
		pointLightShader->setUniform1f("uPointLight[" + std::to_string(i) + "].mBaseLight.mDiffuseIntensity", diffuseIntensity);
		pointLightShader->setUniform3fv("uPointLight[" + std::to_string(i) + "].mPos", lights.first[i]);
		pointLightShader->setUniform1f("uPointLight[" + std::to_string(i) + "].mAttenuation.mConstant", constant);
		pointLightShader->setUniform1f("uPointLight[" + std::to_string(i) + "].mAttenuation.mLinear", linear);
		pointLightShader->setUniform1f("uPointLight[" + std::to_string(i) + "].mAttenuation.mExp", exp);

		float maxChannel = std::fmaxf(std::fmaxf(lights.second[i].x, lights.second[i].y), lights.second[i].z);

		float sphereScale = (-linear + std::sqrtf(linear * linear - 4 * exp * (exp - 256 * maxChannel * diffuseIntensity))) /
							(2 * exp);
		transform.setLocalPosition(lights.first[i]);
		transform.setLocalRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		transform.setLocalScale(glm::vec3(std::fmaxf(1.0f, sphereScale)));

		glm::mat4 WVPMatrix = transform.getWVPTransf(mSceneManager->getProgramProperties().mThirdPersonCam, 
													 mSceneManager->getModelProperties().mProjMatrix);
		pointLightShader->setMatrixUniform4fv("uWVP", WVPMatrix);
		
		mSceneManager->getModelProperties().mModelManager.getModel("sphere").render();
	}
}

void Renderer::directionalLightPass()
{
	auto dirLightShader = &mSceneManager->getProgramProperties().mShaders["dirLight"];
	dirLightShader->bind();
	dirLightShader->setUniform3fv("uViewWorldPos", mSceneManager->getProgramProperties().mThirdPersonCam.getPos());
	glm::mat4 WVPMatrix = glm::mat4(1.0f);
	dirLightShader->setMatrixUniform4fv("uWVP", WVPMatrix);
	
	static uint32_t quadVAO = 0;
	static uint32_t quadVBO = 0;
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions			// texture coords
			-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,		1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		};
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
}

void Renderer::spotLightPass()
{

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
