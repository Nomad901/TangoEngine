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
	//auto gbufferRef = &mSceneManager->mProgramProperties.mGBuffer;
	//
	//gbufferRef->startFrame();
	//
	//geometryPass(gbufferRef);
	//
	//glEnable(GL_STENCIL_TEST);
	//
	//static std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>> lights =
	//	   std::make_pair(mSceneManager->mLightProperties.lightPositions,
	//	   			      mSceneManager->mLightProperties.lightColors);
	//for (size_t i = 0; i < lights.first.size(); ++i)
	//{
	//	// 
	//	// stencil pass is needed for optimization and correcting problems;
	//	// we do not render pixels which are not needed; 
	//	//
	//	stencilPass(gbufferRef, lights, i);
	//	pointLightPass(gbufferRef, lights, i);
	//}
	//
	//glDisable(GL_STENCIL_TEST);
	//
	//directionalLightPass(gbufferRef);
	//finalPass(gbufferRef); 

	//
	// Skybox
	//
	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_LEQUAL);
	//glDisable(GL_BLEND);
	mSceneManager->mProgramProperties.mSkybox->render(mSceneManager->mProgramProperties.mShaders["skyboxShader"]);

	// 
	// Light cubes and fps
	//
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	renderCubeLights();

	Camera& camera = mSceneManager->getProgramProperties().mThirdPersonCam;
	glm::mat4& projMat = mSceneManager->getModelProperties().mProjMatrix;
	Timer& time = mSceneManager->getProgramProperties().mTimer;
	mSceneManager->getModelProperties().mAnimatorManager.getAnimator("bobAnim")->update(camera, projMat, time);
	mSceneManager->getModelProperties().mAnimatorManager.getAnimator("bobAnim")->render();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void Renderer::geometryPass(GBuffer* pGBuffer)
{
	pGBuffer->bindForGeomPass();

	// 
	// preparing brand new depth buffer and color buffer...
	//
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	mSceneManager->mModelProperties.mTerrain->render(&mSceneManager->getProgramProperties().mThirdPersonCam, mSceneManager->mModelProperties.mProjMatrix);

	glDepthMask(GL_FALSE);
}

void Renderer::pointLightPass(GBuffer* pGBuffer, std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>> pStorages,
							  uint32_t pIndex)
{
	pGBuffer->bindForLightPass();

	auto pointLightShader = &mSceneManager->mProgramProperties.mShaders.getShader("pointLight");
	pointLightShader->bind();
	pointLightShader->setUniform3fv("uViewWorldPos", mSceneManager->getProgramProperties().mThirdPersonCam.getPos());
	pointLightShader->setUniform1i("uNumberLightsToProcess", mSceneManager->mLightProperties.lightPositions.size());
	pointLightShader->setUniform2fv("uScreenSize", glm::vec2(mSceneManager->getProgramProperties().mWindowWidth,
															 mSceneManager->getProgramProperties().mWindowHeight));
	pointLightShader->setUniform1i("uPositionMap", 0);
	pointLightShader->setUniform1i("uColorMap", 1);
	pointLightShader->setUniform1i("uNormalMap", 2);

	//
	// we render only pixels, which are not 0 into the stencil buffer;
	//
	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	Transform transform;
	float ambientIntensity = 1.1f;
	float diffuseIntensity = 1.0f;
	float constant = 1.0f;
	float linear = 0.09f;
	float exp = 0.032f;

	pointLightShader->setUniform3fv("uPointLight[" + std::to_string(pIndex) + "].mBaseLight.mColor", pStorages.second[pIndex]);
	pointLightShader->setUniform3fv("uPointLight[" + std::to_string(pIndex) + "].mPos", pStorages.first[pIndex]);
	pointLightShader->setUniform1f("uPointLight[" +  std::to_string(pIndex) + "].mBaseLight.mAmbientIntensity", ambientIntensity);
	pointLightShader->setUniform1f("uPointLight[" +  std::to_string(pIndex) + "].mBaseLight.mDiffuseIntensity", diffuseIntensity);
	pointLightShader->setUniform1f("uPointLight[" +  std::to_string(pIndex) + "].mAttenuation.mConstant", constant);
	pointLightShader->setUniform1f("uPointLight[" +  std::to_string(pIndex) + "].mAttenuation.mLinear", linear);
	pointLightShader->setUniform1f("uPointLight[" +  std::to_string(pIndex) + "].mAttenuation.mExp", exp);

	float maxChannel = std::fmaxf(std::fmaxf(pStorages.second[pIndex].x, pStorages.second[pIndex].y), pStorages.second[pIndex].z);
	float sphereScale = (-linear + std::sqrtf(linear * linear - 4 * exp * (exp - 256 * maxChannel * diffuseIntensity))) /
						(2 * exp);

	transform.setLocalPosition(pStorages.first[pIndex]); 
	transform.setLocalRotation(glm::vec3(0.0f));
	transform.setLocalScale(glm::vec3(std::fmaxf(1.0f, sphereScale)));
	
	glm::mat4 WVPMatrix = transform.getWVPTransf(mSceneManager->getProgramProperties().mThirdPersonCam,
		mSceneManager->getModelProperties().mProjMatrix);
	pointLightShader->setMatrixUniform4fv("uWVP", WVPMatrix);

	mSceneManager->getModelProperties().mModelManager.getModel("sphere").render();

	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}

void Renderer::directionalLightPass(GBuffer* pGBuffer)
{
	pGBuffer->bindForLightPass();
	auto dirLightShader = &mSceneManager->getProgramProperties().mShaders["dirLight"];
	dirLightShader->bind();
	dirLightShader->setUniform3fv("uViewWorldPos", mSceneManager->getProgramProperties().mThirdPersonCam.getPos());
	dirLightShader->setUniform2fv("uScreenSize", glm::vec2(mSceneManager->getProgramProperties().mWindowWidth,
														   mSceneManager->getProgramProperties().mWindowHeight));
	dirLightShader->setUniform1i("uNumberLightsToProcess", mSceneManager->mLightProperties.lightPositions.size());

	dirLightShader->setUniform1i("uPositionMap", 0);
	dirLightShader->setUniform1i("uColorMap", 1);
	dirLightShader->setUniform1i("uNormalMap", 2);
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	Transform transform;
	dirLightShader->setUniform3fv("uDirectionalLight[0].mBaseLight.mColor", glm::vec3(1.0f, 1.0f, 0.9f));
	dirLightShader->setUniform3fv("uDirectionalLight[0].mDirection", mSceneManager->getLightProperties().mLightDir);
	//dirLightShader->setUniform3fv("uDirectionalLight[0].mDirection", glm::vec3(-1.0f, -1.0f, -1.0f));
	dirLightShader->setUniform1f("uDirectionalLight[0].mBaseLight.mAmbientIntensity", mSceneManager->getLightProperties().mAmbientIntensity);
	dirLightShader->setUniform1f("uDirectionalLight[0].mBaseLight.mDiffuseIntensity", mSceneManager->getLightProperties().mDiffuseIntensity);

	static uint32_t quadVAO = 0;
	static uint32_t quadVBO;
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			-1.0f,  1.0f, -1.0f,  
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
		};
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	if (mSceneManager->getProgramProperties().mRenderTheQuadForGBuffer)
	{
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	glDisable(GL_BLEND);
}

void Renderer::stencilPass(GBuffer* pGBuffer, std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>> pStorages, 
						   uint32_t pIndex)
{
	auto shader = &mSceneManager->getProgramProperties().mShaders.getShader("nullShader");
	shader->bind();
	//
	// turns off all write buffers cuz from this moment we need to read from the buffer, instead of writing into it;
	//
	pGBuffer->bindForStencilPass();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_ALWAYS, 0, 0);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
	
	Transform transform;
	float ambientIntensity = 1.1f;
	float diffuseIntensity = 1.0f;
	float constant = 1.0f;
	float linear = 0.09f;
	float exp = 0.032f;

	float maxChannel = std::fmaxf(std::fmaxf(pStorages.second[pIndex].x, pStorages.second[pIndex].y), pStorages.second[pIndex].z);
	float sphereScale = (-linear + std::sqrtf(linear * linear - 4 * exp * (exp - 256 * maxChannel * diffuseIntensity))) /
						(2 * exp);

	transform.setLocalPosition(pStorages.first[pIndex]);
	transform.setLocalRotation(glm::vec3(0.0f));
	transform.setLocalScale(glm::vec3(std::fmaxf(1.0f, sphereScale)));
	glm::mat4 WVPMatrix = transform.getWVPTransf(mSceneManager->getProgramProperties().mThirdPersonCam, mSceneManager->getModelProperties().mProjMatrix);
	shader->setMatrixUniform4fv("uWVP", WVPMatrix);
	mSceneManager->getModelProperties().mModelManager.getModel("sphere").render();
}

void Renderer::finalPass(GBuffer* pGBuffer)
{
	pGBuffer->bindForFinalPass();
	glBlitFramebuffer(0, 0, mSceneManager->getProgramProperties().mWindowWidth, mSceneManager->getProgramProperties().mWindowHeight,
					  0, 0, mSceneManager->getProgramProperties().mWindowWidth, mSceneManager->getProgramProperties().mWindowHeight, 
					  GL_COLOR_BUFFER_BIT, GL_LINEAR);
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