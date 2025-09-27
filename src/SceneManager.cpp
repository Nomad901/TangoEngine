#include "SceneManager.h"
#include "Renderer.h"

void SceneManager::setAll()
{
	setLights();
	setLightCube();
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
	glDisable(GL_CULL_FACE);
	mProgramProperties.mShaders["singleColorShader"].bind();

	mModelProperties.mFactoryMeshes.getMesh("lightCube").initMVP(mModelProperties.mProjMatrix, mProgramProperties.mCamera.getViewMatrix(),
								    mProgramProperties.mCamera.getPos() + mProgramProperties.mCamera.getDirection() * mProgramProperties.mRadius,
								    std::make_pair(glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
								    glm::vec3(1.0f, 1.0f, 1.0f));
	mProgramProperties.mShaders["singleColorShader"].setMatrixUniform4fv("uMVP", mModelProperties.mFactoryMeshes.getMesh("lightCube").getMVP());
	mModelProperties.mFactoryMeshes.getMesh("lightCube").draw();

	mLightProperties.mLightManager.getLight("pointLight1")->get()->setPosLight(mProgramProperties.mCamera.getPos() +
																			   mProgramProperties.mCamera.getDirection() * mProgramProperties.mRadius);
	glEnable(GL_CULL_FACE);
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

}

void SceneManager::setSkybox()
{
	static float pNumber = 1.0f;
	glm::mat4 model = model = glm::scale(glm::vec3(3000.0f, 3000.0f, 3000.0f));
	glm::mat4 view = glm::mat4(glm::mat3(mProgramProperties.mCamera.getViewMatrix()));
	view = glm::rotate(view, glm::radians(pNumber), glm::vec3(0.0f, 1.0f, 0.0f));
	pNumber += 0.01f;

	mProgramProperties.mSkybox->setMVP(model, mModelProperties.mProjMatrix, view);
	mProgramProperties.mSkybox->render(mProgramProperties.mShaders["skyboxShader"]);
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
