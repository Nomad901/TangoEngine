#pragma once
#include <iostream>
#include <format>
#include <filesystem>

#include "Utils.h"
#include "Shader.h"
#include "SkinnedMesh.h"
#include "AnimatorShader.h"

class Animator
{
public:
	Animator() = default;
	Animator(const std::filesystem::path& pModelPath,
			 const std::filesystem::path& pVertShaderPath,
			 const std::filesystem::path& pFragShaderPath,
			 float pTimeOfAnimation);
	~Animator() = default;

	void loadModel(const std::filesystem::path& pModelPath,
				   const std::filesystem::path& pVertShaderPath, 
				   const std::filesystem::path& pFragShaderPath, 
				   float pTimeOfAnimation);

	void stopAnimation();
	void continueAnimation();
	void playAnimationFromBeginning();
	void setTimeOfAnimation(float pTimeOfAnimation);

	void setLightPostions(const std::array<glm::vec3, 2> pPointLightPositions,
						  const std::array<glm::vec3, 2> pSpotLightPositions);
	void setLightDirection(const glm::vec3& pLightDirecion);

	float getTimeOfAnimation() const noexcept;
	bool animationIsPlaying() const noexcept;

	void update(Camera& pCamera, const glm::mat4& pProjMatrix, Timer& pTimer);
	void render();

private:
	float mTimeOfAnim{ 0.0f };
	
	AnimatorShader mAnimatorShader;
	SkinnedMesh mSkinnedMesh;
};

