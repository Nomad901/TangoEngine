#include "SceneManager.h"
#include "Renderer.h"

void SceneManager::setAll()
{
	setLights();
	//setLightCube();
	setMaterials();
	setModels();
	setSkybox();
}

void SceneManager::setLights()
{
	setMaterials();

	mLightProperties.mLightManager.sendAllToShader(mProgramProperties.mShaders["mainShader"]);
	mMaterialProperties.mMaterial->sendToShaderColored(mProgramProperties.mShaders["mainShader"]);

	mLightProperties.mLightManager.sendAllToShader(mProgramProperties.mShaders["singleColorShader"]);
}

void SceneManager::setLightCube()
{
	//mProgramProperties.mShaders["singleColorShader"].bind();
	//
	//mModelProperties.mFactoryMeshes.getMesh("lightBlock").initMVP(mModelProperties.mProjMatrix, mProgramProperties.mViewMatrix,
	//							    mProgramProperties.mCamera.getPos() + mProgramProperties.mCamera.getDirection() * mProgramProperties.mRadius,
	//							    std::make_pair(glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
	//							    glm::vec3(1.0f, 1.0f, 1.0f));
	//mProgramProperties.mShaders["singleColorShader"].setMatrixUniform4fv("uMVP", mModelProperties.mFactoryMeshes.getMesh("lightBlock").getMVP());
	//
	//mLightProperties.mLightManager.getLight("pointLight1")->get()->setPosLight(mProgramProperties.mCamera.getPos() +
	//																		   mProgramProperties.mCamera.getDirection() * mProgramProperties.mRadius);
	//
	// single color shader part
	//mSceneManager->mProgramProperties.mShaders["singleColorShader"].bind();
	//glDisable(GL_CULL_FACE);
	//mSceneManager->mModelProperties.mFactoryMeshes.getMesh("lightBlock").draw();
	//glEnable(GL_CULL_FACE);
}

void SceneManager::setMaterials()
{
	mMaterialProperties.mMaterial->setAmbient(mMaterialProperties.mAmbient);
	mMaterialProperties.mMaterial->setDiffuse(mMaterialProperties.mDiffuse);
	mMaterialProperties.mMaterial->setSpecular(mMaterialProperties.mSpecular);
	mMaterialProperties.mMaterial->setShines(mMaterialProperties.mShines);
}

void SceneManager::setModels()
{
	mProgramProperties.mShaders["mainShader"].bind();
	mProgramProperties.mShaders["mainShader"].setUniform3fv("cameraPos", mProgramProperties.mThirdPersonCam.getPos());
	mMaterialProperties.mMaterial->sendToShaderColored(mProgramProperties.mShaders["mainShader"]);

	//// museum
	//mModelProperties.mModelManager.getModel("museum").initMVP(mModelProperties.mProjMatrix,
	//														  mProgramProperties.mViewMatrix,
	//														  glm::vec3(-257.0f, -65.0f, -503.0f),
	//														  std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
	//														  glm::vec3(9.0f, 11.0f, 9.0f));
	//mModelProperties.mModelManager.getModel("museum").setUniforms(mProgramProperties.mShaders["mainShader"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	//// light posts
	auto setLightModels = [&](const glm::vec3& pPos, std::string_view pName)
		{
			std::string name{ pName };
			mModelProperties.mModelManager[name].initMVP(mModelProperties.mProjMatrix,
														 mProgramProperties.mViewMatrix,
														 pPos, std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
														 glm::vec3(20.0f, 20.0f, 20.0f));
			mModelProperties.mModelManager[name].setUniforms(mProgramProperties.mShaders["mainShader"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		};
	setLightModels(glm::vec3(-140.900051, 25.700062, -78.89937), "lampPost1");
	setLightModels(glm::vec3(22.900051, 25.700062, -78.89937), "lampPost2");
}

void SceneManager::setSkybox()
{
	static float pNumber = 1.0f;
	glm::mat4 model = model = glm::scale(glm::vec3(3000.0f, 3000.0f, 3000.0f));
	glm::mat4 view = glm::mat4(glm::mat3(mProgramProperties.mViewMatrix));
	view = glm::rotate(view, glm::radians(pNumber), glm::vec3(0.0f, 1.0f, 0.0f));
	pNumber += 0.01f;

	mProgramProperties.mSkybox->setMVP(model, mModelProperties.mProjMatrix, view);
}

SceneManager::programProperties& SceneManager::getProgramProperties() noexcept
{
	return mProgramProperties;
}

SceneManager::lightProperties& SceneManager::getLightProperties() noexcept
{
	return mLightProperties;
}

SceneManager::materialProperties& SceneManager::getMaterialProperties() noexcept
{
	return mMaterialProperties;
}

SceneManager::modelProperties& SceneManager::getModelProperties() noexcept
{
	return mModelProperties;
}