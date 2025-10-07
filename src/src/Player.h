#pragma once
#include <iostream>
#include <format>
#include <vector>
#include <expected>

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "thirdPersonCam.h"
#include "Collider.h"
#include "Model.h"
#include "PhysicsEquations.h"
#include "Terrain.h"

enum class states
{
	JUMPING_UP = 0,
	JUMPING_DOWN = 1,
	WALKING = 2,
	SPRINTING = 3,
	SITTING = 4,
	STANDING = 5
};

enum class moveSidesPlayer
{
	RIGHT = 0,
	LEFT = 1,
	FORWARD = 2,
	BACKWARD = 3
};

class Player
{
public:
	Player() = default;
	Player(const glm::vec3& pPos, const glm::vec3& pVelocity, float pSpeed, 
		   float pSprintSpeed, float pJumpForce, bool pThirdPersonCam, 
		   const std::filesystem::path& pPath);

	void init(const glm::vec3& pPos, const glm::vec3& pVelocity, float pSpeed, 
			  float pSprintVelocity, float pJumpForce, bool pThirdPersonCam, 
			  const std::filesystem::path& pPath);

	void move(moveSidesPlayer pMoveSidesPlayer, float pDeltaTime);
	void jump();
	void sprint(bool pSprint);

	void turnOnNoclip(bool pNoclip);
	void freezePlayer(bool pFreeze);
	void turnOn3rdPersonCamera(bool p3rdPersonCamera);

	//
	// that means, that when character will be moving in different directions (rotating)
	// the camera will be always behind the character. This will happen if pRotatingWithChar is true.
	// else - only character will be rotated
	//
	void turnOnRotatingWithCharacter(bool pRotatingWithChar);

	void update(const glm::mat4& pProjMatrix, float pDeltaTime, const std::vector<Mesh*>& pCollisionMeshes,
			    Terrain& pTerrain);
	void renderCharacter(Shader& pShader);
	void checkCollisions(const std::vector<Mesh*>& pCollisionMeshes);

	void setPos(const glm::vec3& pPos);
	void updateEvents(const SDL_Event& pEvent);

	Mesh& getHitbox() noexcept;
	Camera& getCamera() noexcept;
	thirdPersonCam& getThirdPersonCamera() noexcept;
	glm::vec3 getPos() const noexcept;
	float getSpeed() const noexcept;
	float getSprintSpeed() const noexcept;
	float getRotationY() const noexcept;
	bool ifRotatesWithCamera() const noexcept;
	bool isInThirdPersonCamera() const noexcept;

private:
	bool isOnGround() const noexcept;
	void responseCollision(Mesh& pObstacle);

private:
	bool m3rdPersonCamera{ false };
	bool mRotateCameraWithChar{ false };
	bool mNoclip{ false };
	bool mIsFreezed{ false };

	bool mIsGrounded{ false };
	bool mIsJumping{ false };
	bool mIsSprinting{ false };
	states mCurrentState{ states::STANDING };

	float mCurrentSpeed{ 0.0f };
	float mCurrentTurnSpeed{ 0.0f };
	float mUpWardsSpeed{ 0.0f };
	float mTurnSpeed{ 90.0f };
	float mRotationY{ 0.0f };
	float mJumpForce{ 1.0f };
	float mMoveSpeed{ 0.5f };
	float mSprintSpeed{ 1.0f };
	float mGravity{ -9.81f };
	float mFrictionGround{ 0.1f };
	float mFrictionAir{ 0.2f };
	float mTerrainHeight{ 0.0f };

	glm::vec3 mPos{ 1.0f };
	glm::vec3 mVelocity{ 1.0f };
	
	glm::vec3 mInputVelocity{ 0.0f };
	glm::vec3 mPhysicsVelocity{ 0.0f };

	Collider mCollider;
	Mesh mPlayerHitbox;
	Model mCharModel;
	Camera mCamera;
	thirdPersonCam mThirdPersonCam;
	SDL_Event mEvents;
};

