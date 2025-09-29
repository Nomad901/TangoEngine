#pragma once
#include <iostream>
#include <format>
#include <vector>
#include <expected>

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Camera.h"
#include "Collider.h"
#include "Model.h"
#include "PhysicsEquations.h"

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
		   float pSprintSpeed, float pJumpForce);

	void init(const glm::vec3& pPos, const glm::vec3& pVelocity, float pSpeed, 
			  float pSprintVelocity, float pJumpForce);

	void move(moveSidesPlayer pMoveSidesPlayer, float pDeltaTime);
	void jump();
	void sprint(bool pSprint);

	void update(const glm::mat4& pProjMatrix, float pDeltaTime, const std::vector<Mesh*>& pCollisionMeshes);
	
	void turnOnNoclip(bool pNoclip);

	void setPos(const glm::vec3& pPos);

	Mesh& getHitbox() noexcept;
	Camera& getCamera() noexcept;
	glm::vec3 getPos() const noexcept;
	float getSpeed() const noexcept;
	float getSprintSpeed() const noexcept;

private:
	bool isOnGround() const noexcept;

private:
	bool mNoclip;
	bool mIsGrounded{ false };
	bool mIsJumping{ false };
	bool mIsSprinting{ false };
	states mCurrentState{ states::STANDING };

	float mJumpForce{ 1.0f };
	float mMoveSpeed{ 1.0f };
	float mSprintSpeed{ 1.0f };
	float mGravity{ -9.81f };
	float mFrictionGround{ 0.1f };
	float mFrictionAir{ 0.2f };

	glm::vec3 mPos{ 1.0f };
	glm::vec3 mVelocity{ 1.0f };
	
	glm::vec3 mInputVelocity{ 0.0f };
	glm::vec3 mPhysicsVelocity{ 0.0f };

	float mUpSpeed{ 0.0f };

	Collider mCollider;
	Mesh mPlayerHitbox;
	Camera mCamera;
};

