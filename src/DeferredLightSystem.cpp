#include "DeferredLightSystem.h"

DeferredLightSystem::DeferredLightSystem(uint32_t pWidthWindow, uint32_t pHeightWindow,
	const std::vector<std::pair<glm::vec3, glm::vec3>> pLightPositionsAndColors)
{
	init(pWidthWindow, pHeightWindow, pLightPositionsAndColors);
}

void DeferredLightSystem::init(uint32_t pWidthWindow, uint32_t pHeightWindow,
	const std::vector<std::pair<glm::vec3, glm::vec3>> pLightPositionsAndColors)
{
	mGBuffer.init(pWidthWindow, pHeightWindow);
	mLightPositions.reserve(pLightPositionsAndColors.size());
	mLightColors.reserve(pLightPositionsAndColors.size());

	for (auto& i : pLightPositionsAndColors)
	{
		mLightPositions.push_back(i.first);
		mLightColors.push_back(i.second);
	}

	std::string resourcePath = RESOURCES_PATH;
	mShaders[getIndex(TypeShader::NULL_SHADER)] = std::make_unique<Shader>(resourcePath + "Shaders/nullShaderVert.glsl",
		resourcePath + "Shaders/nullShaderFrag.glsl");
	mShaders[getIndex(TypeShader::LIGHT_SHADER)] = std::make_unique<Shader>(resourcePath + "Shaders/lightPassVert.glsl",
		resourcePath + "Shaders/pointLightPassFrag.glsl");
	mShaders[getIndex(TypeShader::FRAME_SHADER)] = std::make_unique<Shader>(resourcePath + "Shaders/lightPassVert.glsl",
		resourcePath + "Shaders/dirLightPassFrag.glsl");

	mSphereModel = std::make_unique<Model>(glm::vec3(1.0f), resourcePath + "Models/sphere.obj");

	mWidthWindow = pWidthWindow;
	mHeightWindow = pHeightWindow;

	float quadVertices[] = 
	{
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		 1.0f, -1.0f,

		-1.0f,  1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f
	};
	glGenVertexArrays(1, &mQuadVAO);
	glGenBuffers(1, &mQuadVBO);
	glBindVertexArray(mQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
}

void DeferredLightSystem::startGeometryPass()
{
	mGBuffer.startFrame();

	geometryPassBeginning();
}

void DeferredLightSystem::stopGeometryPass()
{
	geometryPassEnding();
}

void DeferredLightSystem::startLightPass(Camera pCamera, const glm::mat4& pProjMatrix)
{
	glEnable(GL_STENCIL_TEST);

	for (size_t i = 0; i < mLightPositions.size(); ++i)
	{
		stencilPass(i, pCamera, pProjMatrix);
		lightPass(i, pCamera, pProjMatrix);
	}

	glDisable(GL_STENCIL_TEST);

	framePass(pCamera, pProjMatrix);
}

void DeferredLightSystem::stopLightPassAndRenderFrame()
{
	finalPass();
}

void DeferredLightSystem::geometryPassBeginning()
{
	mGBuffer.bindForGeomPass();

	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void DeferredLightSystem::geometryPassEnding()
{
	glDepthMask(GL_FALSE);
}

void DeferredLightSystem::stencilPass(size_t pIndex, Camera pCamera, const glm::mat4& pProjMatrix)
{
	auto nullShader = &mShaders[getIndex(TypeShader::NULL_SHADER)];
	nullShader->get()->bind();

	mGBuffer.bindForStencilPass();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_ALWAYS, 0, 0);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	Transform transform;
	float ambientIntensity = 4.1f;
	float diffuseIntensity = 10.0f;
	float constant = 1.0f;
	float linear = -10.0f;
	float exp = 0.032f;
	float maxChannel = std::fmaxf(std::fmaxf(mLightColors[pIndex].x, mLightColors[pIndex].y), mLightColors[pIndex].z);
	float sphereScale = (-linear + std::sqrtf(linear * linear - 4 * exp * (exp - 256 * maxChannel * diffuseIntensity))) /
		(2 * exp);

	transform.setLocalPosition(mLightPositions[pIndex]);
	transform.setLocalRotation(glm::vec3(0.0f));
	transform.setLocalScale(glm::vec3(sphereScale));
	glm::mat4 WVPMatrix = transform.getWVPTransf(pCamera, pProjMatrix);
	nullShader->get()->setMatrixUniform4fv("uWVP", WVPMatrix);

	mSphereModel->render();
}

void DeferredLightSystem::lightPass(size_t pIndex, Camera pCamera, const glm::mat4& pProjMatrix)
{
	mGBuffer.bindForLightPass();

	auto lightShader = &mShaders[getIndex(TypeShader::LIGHT_SHADER)];
	lightShader->get()->bind();
	lightShader->get()->setUniform3fv("uViewWorldPos", pCamera.getPos());
	lightShader->get()->setUniform1i("uNumberLightsToProcess", mLightPositions.size());
	lightShader->get()->setUniform2fv("uScreenSize", glm::vec2(mWidthWindow, mHeightWindow));
	lightShader->get()->setUniform1i("uPositionMap", 0);
	lightShader->get()->setUniform1i("uColorMap", 1);
	lightShader->get()->setUniform1i("uNormalMap", 2);

	//
	// we render only pixels, which are not 0 into the stencil buffer;
	//
	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Transform transform;
	float ambientIntensity = 4.1f;
	float diffuseIntensity = 4.0f;
	float constant = 1.0f;
	float linear = 0.09f;
	float exp = 0.032f;

	lightShader->get()->setUniform3fv("uPointLight[" + std::to_string(pIndex) + "].mBaseLight.mColor", mLightColors[pIndex]);
	lightShader->get()->setUniform3fv("uPointLight[" + std::to_string(pIndex) + "].mPos", mLightPositions[pIndex]);
	lightShader->get()->setUniform1f("uPointLight[" + std::to_string(pIndex) + "].mBaseLight.mAmbientIntensity", ambientIntensity);
	lightShader->get()->setUniform1f("uPointLight[" + std::to_string(pIndex) + "].mBaseLight.mDiffuseIntensity", diffuseIntensity);
	lightShader->get()->setUniform1f("uPointLight[" + std::to_string(pIndex) + "].mAttenuation.mConstant", constant);
	lightShader->get()->setUniform1f("uPointLight[" + std::to_string(pIndex) + "].mAttenuation.mLinear", linear);
	lightShader->get()->setUniform1f("uPointLight[" + std::to_string(pIndex) + "].mAttenuation.mExp", exp);

	float maxChannel = std::fmaxf(std::fmaxf(mLightColors[pIndex].x, mLightColors[pIndex].y), mLightColors[pIndex].z);
	float sphereScale = (-linear + std::sqrtf(linear * linear - 4 * exp * (exp - 256 * maxChannel * diffuseIntensity))) /
		(2 * exp);

	lightShader->get()->setUniform1f("uSphereScale", sphereScale);

	renderQuad();

	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}

void DeferredLightSystem::renderQuad()
{
	glBindVertexArray(mQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void DeferredLightSystem::framePass(Camera pCamera, const glm::mat4& pProjMatrix)
{
	mGBuffer.bindForLightPass();

	auto frameShader = &mShaders[getIndex(TypeShader::FRAME_SHADER)];
	frameShader->get()->bind();
	frameShader->get()->setUniform3fv("uViewWorldPos", pCamera.getPos());
	frameShader->get()->setUniform2fv("uScreenSize", glm::vec2(mWidthWindow, mHeightWindow));

	frameShader->get()->setUniform1i("uPositionMap", 0);
	frameShader->get()->setUniform1i("uColorMap", 1);
	frameShader->get()->setUniform1i("uNormalMap", 2);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	float ambientIntensity = 0.1f;
	float diffuseIntensity = 0.1f;
	glm::vec3 baseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 directionLight = glm::vec3(1.0f, 1.0f, 1.0f);

	Transform transform;
	frameShader->get()->setUniform3fv("uDirectionalLight[0].mBaseLight.mColor", baseColor);
	frameShader->get()->setUniform3fv("uDirectionalLight[0].mDirection", directionLight);
	frameShader->get()->setUniform1f("uDirectionalLight[0].mBaseLight.mAmbientIntensity", ambientIntensity);
	frameShader->get()->setUniform1f("uDirectionalLight[0].mBaseLight.mDiffuseIntensity", diffuseIntensity);

	renderQuad();

	glDisable(GL_BLEND);
}

void DeferredLightSystem::finalPass()
{
	mGBuffer.bindForFinalPass();
	glBlitFramebuffer(0, 0, mWidthWindow, mHeightWindow,
		0, 0, mWidthWindow, mHeightWindow,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

uint32_t DeferredLightSystem::getIndex(TypeShader pTypeShader)
{
	return static_cast<uint32_t>(pTypeShader);
}