#include "AnimatorShader.h"
#include "SkinnedMesh.h"

AnimatorShader::AnimatorShader(const std::filesystem::path& pVertPath,
							   const std::filesystem::path& pFragPath)
{
	init(pVertPath, pFragPath);
}

void AnimatorShader::init(const std::filesystem::path& pVertPath,
						  const std::filesystem::path& pFragPath)
{
	mShader.init(pVertPath, pFragPath);
	mPointLightPos = { glm::vec3(10.0f, 10.0f, 10.0f),
					   glm::vec3(20.0f, 20.0f, 20.0f) };
	mSpotLightPos = { glm::vec3(15.0f, 15.0f, 15.0f),
					  glm::vec3(25.0f, 25.0f, 25.0f) };
}

AnimatorShader::STATE_ANIM AnimatorShader::getAnimState() const noexcept
{
	return mStateAnim;
}

float AnimatorShader::getAnimTime() const noexcept
{
	return mAnimTime;
}

void AnimatorShader::accelerateAnim()
{
	mAnimAccelerator /= 10.0f;
}

void AnimatorShader::slowAnim()
{
	mAnimAccelerator *= 10.0f;
}

void AnimatorShader::setLightPostions(const std::array<glm::vec3, 2> pPointLightPositions,
									  const std::array<glm::vec3, 2> pSpotLightPositions)
{
	mPointLightPos = pPointLightPositions;
	mSpotLightPos = pSpotLightPositions;
}

void AnimatorShader::setLightDirection(const glm::vec3& pLightDirecion)
{
	mLightDirection = pLightDirecion;
}

void AnimatorShader::setStateOfAnimation(STATE_ANIM pStateAnim)
{
	mStateAnim = pStateAnim;
}

AnimatorShader::STATE_ANIM AnimatorShader::getAnimationState() const noexcept
{
	return mStateAnim;
}

void AnimatorShader::update(Camera& pCamera, const glm::mat4& pProjMatrix, 
							SkinnedMesh& pSkinnedMesh, Timer& pTimer)
{
	setShader(pCamera, pProjMatrix, pSkinnedMesh, pTimer);
}

void AnimatorShader::setShader(Camera& pCamera, const glm::mat4& pProjMatrix, 
							   SkinnedMesh& pSkinnedMesh, Timer& pTimer)
{
	mShader.bind();

	setTransformations(pCamera, pProjMatrix, pSkinnedMesh.getTransform());
	setLights();
	setMaterial(pSkinnedMesh);
	setCamera(pCamera);
	setBones(pSkinnedMesh, pTimer);
}

void AnimatorShader::setTransformations(Camera& pCamera, const glm::mat4& pProjMatrix,
										Transform& pTransform)
{
	pTransform.setLocalRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
	pTransform.setLocalPosition(glm::vec3(0.0f, 30.0f, 0.0f));
	pTransform.setLocalScale(glm::vec3(1.0f, 1.0f, 1.0f));

	glm::mat4 WVP = pTransform.getWVPTransf(pCamera, pProjMatrix);
	mShader.setMatrixUniform4fv("uMVP", WVP);
}

void AnimatorShader::setLights()
{
	mShader.setUniform1i("uNumberPointLights", mPointLightPos.size());
	mShader.setUniform1i("uNumberSpotLights", mSpotLightPos.size());

	for (size_t i = 0; i < mPointLightPos.size(); ++i)
	{
		mShader.setUniform3fv("uPointLight[" + std::to_string(i) + "].mBaseLight.mColor", glm::vec3(1.0f, 1.0f, 1.0f));
		mShader.setUniform1f("uPointLight[" + std::to_string(i) + "].mBaseLight.mAmbientIntensity", 0.3f);
		mShader.setUniform1f("uPointLight[" + std::to_string(i) + "].mBaseLight.mDiffuseIntensity", 0.8f);

		mShader.setUniform1f("uPointLight[" + std::to_string(i) + "].mAttenuation.mConstant", 1.0f);
		mShader.setUniform1f("uPointLight[" + std::to_string(i) + "].mAttenuation.mLinear", 0.09f);
		mShader.setUniform1f("uPointLight[" + std::to_string(i) + "].mAttenuation.mExp", 0.032f);

		mShader.setUniform3fv("uPointLight[" + std::to_string(i) + "].mPos", mPointLightPos[i]);
	}

	for (size_t i = 0; i < mSpotLightPos.size(); ++i)
	{
		mShader.setUniform3fv("uSpotLight[" + std::to_string(i) + "].mPointLight.mBaseLight.mColor", glm::vec3(1.0f, 1.0f, 1.0f));
		mShader.setUniform1f("uSpotLight[" + std::to_string(i) + "].mPointLight.mBaseLight.mAmbientIntensity", 0.2f);
		mShader.setUniform1f("uSpotLight[" + std::to_string(i) + "].mPointLight.mBaseLight.mDiffuseIntensity", 1.0f);

		mShader.setUniform1f("uSpotLight[" + std::to_string(i) + "].mPointLight.mAttenuation.mConstant", 1.0f);
		mShader.setUniform1f("uSpotLight[" + std::to_string(i) + "].mPointLight.mAttenuation.mLinear", 0.09f);
		mShader.setUniform1f("uSpotLight[" + std::to_string(i) + "].mPointLight.mAttenuation.mExp", 0.032f);

		mShader.setUniform3fv("uSpotLight[" + std::to_string(i) + "].mPointLight.mPos", mSpotLightPos[i]);

		mShader.setUniform3fv("uSpotLight[" + std::to_string(i) + "].mDirection", mLightDirection);

		mShader.setUniform1f("uSpotLight[" + std::to_string(i) + "].mCutOff", glm::cos(glm::radians(12.5f)));
	}

	mShader.setUniform3fv("uDirectionalLight.mBaseLight.mColor", glm::vec3(1.0f, 1.0f, 1.0f));
	mShader.setUniform1f("uDirectionalLight.mBaseLight.mAmbientIntensity", 0.4f);
	mShader.setUniform1f("uDirectionalLight.mBaseLight.mDiffuseIntensity", 0.6f);
	mShader.setUniform3fv("uDirectionalLight.mDirection", glm::vec3(-0.5f, -1.0f, -0.5f));
}

void AnimatorShader::setMaterial(SkinnedMesh& pSkinnedMesh)
{
	mShader.setUniform3fv("uMaterial.mAmbientColor", pSkinnedMesh.getMaterial().mAmbientColor);
	mShader.setUniform3fv("uMaterial.mDiffuseColor", pSkinnedMesh.getMaterial().mDiffuseColor);
	mShader.setUniform3fv("uMaterial.mSpecularColor", pSkinnedMesh.getMaterial().mSpecularColor);
}

void AnimatorShader::setCamera(Camera& pCamera)
{
	mShader.setUniform3fv("uCameraPos", pCamera.getPos());
}

void AnimatorShader::setBones(SkinnedMesh& pSkinnedMesh, Timer& pTimer)
{
	if (mStateAnim != STATE_ANIM::STOP)
	{
		mAnimTime = pTimer.getDeltaTime(false) / mAnimAccelerator;

		std::vector<glm::mat4> bonesTransformations;
		pSkinnedMesh.getBoneTransformations(mAnimTime, bonesTransformations);

		size_t i = 0;
		if (mStateAnim == STATE_ANIM::CONTINUE)
		{
			i = mLastTickAnim;
			mStateAnim = STATE_ANIM::PLAY;
		}
		
		for ( ; i < bonesTransformations.size(); ++i)
		{
			mShader.setMatrixUniform4fv("uBones[" + std::to_string(i) + ']', bonesTransformations[i]);
			mLastTickAnim = i;
		}
	}
}
