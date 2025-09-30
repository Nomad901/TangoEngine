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
	mProgramProperties.mShaders["mainShader"].setUniform3fv("cameraPos", mProgramProperties.mCamera.getPos());
	mMaterialProperties.mMaterial->sendToShaderColored(mProgramProperties.mShaders["mainShader"]);

	//// floor
	mModelProperties.mFactoryMeshes.getMesh("floor").initMVP(mModelProperties.mProjMatrix,
		mProgramProperties.mViewMatrix,
		glm::vec3(1.0f, 0.0f, -23.0f),
		std::make_pair(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
		glm::vec3(3000.0f, 3000.0f, 1.0f));
	mModelProperties.mFactoryMeshes.getMesh("floor").setUniforms(mProgramProperties.mShaders["mainShader"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

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


/*
	FlashLight pos (first - up room. second - down room): 
	Character pos: 9.7801485/14.863845/-304.78183
	Character pos: 36.64512/-174.74876/-308.76395
	spotlight:
	Character pos: 10.37517/19.467102/-428.13168
	Character pos: 24.842197/-176.02649/-506.9026
	point:
	Character pos: 9.424553/20.513628/-551.026
	Character pos: 21.026344/-173.36757/-723.678
	directional:
	Character pos: 12.962267/10.982218/-666.49585
	Character pos: 18.512012/-174.62334/-917.03394
	main:
	Character pos: -64.99592/27.617996/-709.5072
	Character pos: -42.084026/-176.44987/-1019.4506
	area:
	Character pos: -146.69879/20.236301/-667.29083
	Character pos: -129.31462/-175.0184/-914.53455
	emissive:
	Character pos: -146.52676/15.648167/-552.51
	Character pos: -124.68435/-176.2005/-714.96747
	multiple:
	Character pos: -136.55302/15.825421/-432.32208
	Character pos: -120.534386/-175.43192/-499.6635
	attenuation:
	Character pos: -135.90807/16.439344/-306.9898
	Character pos: -117.718864/-177.81673/-306.48254
*/