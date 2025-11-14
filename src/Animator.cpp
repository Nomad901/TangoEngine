#include "Animator.h"

Animator::Animator(const std::filesystem::path& pModelPath,
				   const std::filesystem::path& pVertShaderPath,
				   const std::filesystem::path& pFragShaderPath, 
				   float pTimeOfAnimation)
{
	loadModel(pModelPath, pVertShaderPath, pFragShaderPath, pTimeOfAnimation);
}

void Animator::loadModel(const std::filesystem::path& pModelPath,
						 const std::filesystem::path& pVertShaderPath,
						 const std::filesystem::path& pFragShaderPath, 
						 float pTimeOfAnimation)
{
	mSkinnedMesh.loadMesh(pModelPath);
	mAnimatorShader.init(pVertShaderPath, pFragShaderPath);
	mTimeOfAnim = pTimeOfAnimation;
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

void Animator::setTimeOfAnimation(float pTimeOfAnimation)
{
	mTimeOfAnim = pTimeOfAnimation;
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

void Animator::update(Camera& pCamera, const glm::mat4& pProjMatrix, Timer& pTimer)
{
	mAnimatorShader.update(pCamera, pProjMatrix, mSkinnedMesh, pTimer);
}

void Animator::render()
{
	mSkinnedMesh.render();
}