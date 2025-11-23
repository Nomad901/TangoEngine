#include "Animator.h"

Animator::Animator(const std::filesystem::path& pModelPath,
				   const std::filesystem::path& pVertShaderPath,
				   const std::filesystem::path& pFragShaderPath, 
				   const Transform& pModelTransform)
{
	loadModel(pModelPath, pVertShaderPath, 
			  pFragShaderPath, pModelTransform);
}

void Animator::loadModel(const std::filesystem::path& pModelPath,
						 const std::filesystem::path& pVertShaderPath,
						 const std::filesystem::path& pFragShaderPath,
						 const Transform& pModelTransform)
{
	mSkinnedMesh.loadMesh(pModelPath);
	mAnimatorShader.init(pVertShaderPath, pFragShaderPath);
	mSkinnedMesh.getTransform() = pModelTransform;
}

void Animator::stopAnimation()
{
	mAnimatorShader.setStateOfAnimation(AnimatorShader::STATE_ANIM::STOP);
}

void Animator::continueAnimation()
{
	mAnimatorShader.setStateOfAnimation(AnimatorShader::STATE_ANIM::CONTINUE);
}

void Animator::playAnimationFromBeginning()
{
	mAnimatorShader.setStateOfAnimation(AnimatorShader::STATE_ANIM::PLAY);
}

void Animator::setLightPostions(const std::array<glm::vec3, 2> pPointLightPositions, 
								const std::array<glm::vec3, 2> pSpotLightPositions)
{
	mAnimatorShader.setLightPostions(pPointLightPositions, pSpotLightPositions);
}

void Animator::setLightDirection(const glm::vec3& pLightDirecion)
{
	mAnimatorShader.setLightDirection(pLightDirecion);
}

float Animator::getTimeOfAnimation() const noexcept
{
	return mAnimatorShader.getAnimTime();
}

bool Animator::animationIsPlaying() const noexcept
{
	return mAnimatorShader.getAnimationState() == AnimatorShader::STATE_ANIM::PLAY;
}

Transform& Animator::getModelTransformation() noexcept
{
	return mSkinnedMesh.getTransform();
}

void Animator::update(Camera& pCamera, const glm::mat4& pProjMatrix, Timer& pTimer)
{
	if (mAnimatorShader.getAnimationState() == AnimatorShader::STATE_ANIM::PLAY ||
		mAnimatorShader.getAnimationState() == AnimatorShader::STATE_ANIM::CONTINUE)
	{
		mAnimatorShader.update(pCamera, pProjMatrix, mSkinnedMesh, pTimer);
	}
}

void Animator::render()
{
	mSkinnedMesh.render();
}