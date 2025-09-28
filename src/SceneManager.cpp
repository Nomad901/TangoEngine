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

	mModelProperties.mFactoryMeshes.getMesh("lightBlock").initMVP(mModelProperties.mProjMatrix, mProgramProperties.mCamera.getViewMatrix(),
								    mProgramProperties.mCamera.getPos() + mProgramProperties.mCamera.getDirection() * mProgramProperties.mRadius,
								    std::make_pair(glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
								    glm::vec3(1.0f, 1.0f, 1.0f));
	mProgramProperties.mShaders["singleColorShader"].setMatrixUniform4fv("uMVP", mModelProperties.mFactoryMeshes.getMesh("lightBlock").getMVP());
	mModelProperties.mFactoryMeshes.getMesh("lightBlock").draw();

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
	mProgramProperties.mShaders["mainShader"].bind();
	mProgramProperties.mShaders["mainShader"].setUniform3fv("cameraPos", mProgramProperties.mCamera.getPos());
	mMaterialProperties.mMaterial->sendToShaderColored(mProgramProperties.mShaders["mainShader"]);

	//// floor
	mModelProperties.mFactoryMeshes.getMesh("floor").initMVP(mModelProperties.mProjMatrix,
		mProgramProperties.mCamera.getViewMatrix(),
		glm::vec3(1.0f, 1.0f, -23.0f),
		std::make_pair(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
		glm::vec3(3000.0f, 3000.0f, 1.0f));
	mModelProperties.mFactoryMeshes.getMesh("floor").setUniforms(mProgramProperties.mShaders["mainShader"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	//// museum
	mModelProperties.mModelManager.getModel("museum").initMVP(mModelProperties.mProjMatrix,
		mProgramProperties.mCamera.getViewMatrix(),
		glm::vec3(1.0f, -34.5f, -1147.0f),
		std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
		glm::vec3(1.0f, 1.0f, 1.0f));
	mModelProperties.mModelManager.getModel("museum").setUniforms(mProgramProperties.mShaders["mainShader"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	//// light posts
	auto setLightModels = [&](const glm::vec3& pPos, std::string_view pName)
		{
			mModelProperties.mModelManager[std::string(pName)].initMVP(mModelProperties.mProjMatrix,
				mProgramProperties.mCamera.getViewMatrix(),
				pPos, std::make_pair(1.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
				glm::vec3(20.0f, 20.0f, 20.0f));
			mModelProperties.mModelManager[std::string(pName)].setUniforms(mProgramProperties.mShaders["mainShader"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		};
	setLightModels(glm::vec3(1.0f, -20.0f, 150.0f), "lampPost1");
	setLightModels(glm::vec3(22.900051, 25.700062, -78.89937), "lampPost2");
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
