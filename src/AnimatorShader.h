#pragma once
#include <array>

#include "glm.hpp"

#include "Shader.h"
#include "thirdPersonCam.h"
#include "Transform.h"
#include "Timer.h" 

class SkinnedMesh;

class AnimatorShader
{
public:
	// 
	// stop - animation is stoped
	// continue - animation is continued from the stop point
	// play - animation will be playing from the beginning
	//
	enum class STATE_ANIM : uint32_t
	{
		STOP = 0,
		CONTINUE = 1,
		PLAY = 2
	};
public:
	AnimatorShader() = default;
	AnimatorShader(const std::filesystem::path& pVertPath, 
				   const std::filesystem::path& pFragPath);

	void init(const std::filesystem::path& pVertPath,
			  const std::filesystem::path& pFragPath);

	STATE_ANIM getAnimState() const noexcept;
	float getAnimTime() const noexcept;
	
	//
	// accelerateAnim - decreases mAnimAccelerator by dividing it by 10.0f;
	// slowAnim - increases mAnimAccelerator by multiplying it by 10.0f;
	// 
	// during updating deltaTime of the program will be divided by mAnimAccelerator;
	// than mAnimAccelerator bigger, then animation will be going slower and conversely;
	//
	void accelerateAnim();
	void slowAnim();
	void setAnimAccelerator(float pAnimAccelerator);
	float getAnimAccelerator() const noexcept;

	void setLightPostions(const std::array<glm::vec3, 2> pPointLightPositions,
						  const std::array<glm::vec3, 2> pSpotLightPositions);
	void setLightDirection(const glm::vec3& pLightDirecion);
	
	void setStateOfAnimation(STATE_ANIM pStateAnim);
	STATE_ANIM getAnimationState() const noexcept;

	void update(Camera& pCamera, const glm::mat4& pProjMatrix, 
				SkinnedMesh& pSkinnedMesh, Timer& pTimer);

private:
	void setShader(Camera& pCamera, const glm::mat4& pProjMatrix, 
				   SkinnedMesh& pSkinnedMesh, Timer& pTimer);

	void setTransformations(Camera& pCamera, const glm::mat4& pProjMatrix,
							Transform& pTransform);
	void setLights();
	void setMaterial(SkinnedMesh& pSkinnedMesh);
	void setCamera(Camera& pCamera);
	void setBones(SkinnedMesh& pSkinnedMesh, Timer& pTimer);

private:
	float mAnimTime{ 0.0f };
	float mAnimAccelerator{ 1000.0f };
	float mLastTickAnim{ 0.0f };
	STATE_ANIM mStateAnim{ STATE_ANIM::PLAY };

	std::array<glm::vec3, 2> mPointLightPos;
	std::array<glm::vec3, 2> mSpotLightPos;
	glm::vec3 mLightDirection;

	Shader mShader;
};

