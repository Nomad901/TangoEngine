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
	Player(const glm::vec3& pPos, const glm::vec3& pVelocity, const glm::vec3& pSprintVelocity, float pJumpForce);

	void init(const glm::vec3& pPos, const glm::vec3& pVelocity, const glm::vec3& pSprintVelocity, float pJumpForce);

	void move(moveSidesPlayer pMoveSidesPlayer);
	void jump(bool pIsJumping);
	void sit(bool pIsSitting);
	void sprint(bool pSprint);

	void update(const glm::mat4& pProjMatrix, float pDeltaTime, const std::vector<Mesh*>& pCollisionMeshes);

	void setCollide(bool pCollides);

	Mesh& getHitbox() noexcept;
	Camera& getCamera() noexcept;
	glm::vec3 getPos() const noexcept;
	glm::vec3 getVelocity() const noexcept;
	bool isCollided() const noexcept;

private:
	void solveCollision(const Mesh& pMesh);

private:
	bool mIsGrounded{ true };
	bool mIsCollided{ false };
	bool mIsSitting{ false };
	bool mIsJumping{ false };
	float mJumpForce{ 1.0f };

	glm::vec3 mPos{ 1.0f };
	glm::vec3 mVelocity{ 1.0f };
	glm::vec3 mSprintVelocity{ 1.0f };
	glm::vec3 mOrigVelocity{ 1.0f };

	Collider mCollider;
	Mesh mPlayerHitbox;
	Camera mCamera;
};

