#pragma once
#include <iostream>
#include <format>
#include <vector>

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Camera.h"
#include "Collider.h"
#include "Model.h"
#include "PhysicsEquations.h"

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

	Mesh& getHitbox() noexcept;
	Camera& getCamera() noexcept;
	glm::vec3 getPos() const noexcept;
	float getSpeed() const noexcept;
	float getSprintSpeed() const noexcept;

private:
	void solveCollision(const Mesh& pMesh);

private:
	bool mIsGrounded{ false };
	bool mIsJumping{ false };
	bool mIsSprinting{ false };
	
	float mJumpForce{ 1.0f };
	float mMoveSpeed{ 1.0f };
	float mSprintSpeed{ 1.0f };
	float mGravity{ -9.81f };
	float mFriction{ 1.0f };

	glm::vec3 mPos{ 1.0f };
	glm::vec3 mVelocity{ 1.0f };
	
	glm::vec3 mInputVelocity{ 0.0f };
	glm::vec3 mPhysicsVelocity{ 0.0f };

	Collider mCollider;
	Mesh mPlayerHitbox;
	Camera mCamera;
};

