#include "Player.h"

Player::Player(const glm::vec3& pPos, const glm::vec3& pVelocity, const glm::vec3& pSprintVelocity, float pJumpForce)
{
	init(pPos, pVelocity, pSprintVelocity, pJumpForce);
}

void Player::init(const glm::vec3& pPos, const glm::vec3& pVelocity, const glm::vec3& pSprintVelocity, float pJumpForce)
{
	mJumpForce = pJumpForce;
	mPos = pPos;
	mVelocity = pVelocity;
	mSprintVelocity = pSprintVelocity;
	mOrigVelocity = mVelocity;
	std::shared_ptr<Primitive> hitbox = std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	std::weak_ptr weakHitbox = hitbox;
	mPlayerHitbox.init(hitbox);
}

void Player::move(moveSidesPlayer pMoveSidesPlayer)
{
	switch (pMoveSidesPlayer)
	{
	case moveSidesPlayer::RIGHT:
		mCamera.moveCamera(moveSides::RIGHT, mVelocity.x);
		break;
	case moveSidesPlayer::LEFT:
		mCamera.moveCamera(moveSides::LEFT, mVelocity.x);
		break;
	case moveSidesPlayer::FORWARD:
		mCamera.moveCamera(moveSides::FORWARD, mVelocity.z);
		break;
	case moveSidesPlayer::BACKWARD:
		mCamera.moveCamera(moveSides::BACKWARD, mVelocity.z);
		break;
	}
}

void Player::jump(bool pIsJumping)
{
	if (pIsJumping && !mIsJumping && mIsGrounded)
	{
		mVelocity.y = mJumpForce;
		mIsGrounded = false;
		mIsJumping = true;
	}
	else if (!pIsJumping)
		mIsJumping = false;
}

void Player::sit(bool pIsSitting)
{
	mIsSitting = pIsSitting;
}

void Player::sprint(bool pSprint)
{
	if (pSprint)
		mVelocity = mSprintVelocity;
	else
		mVelocity = mOrigVelocity;
}

void Player::update(const glm::mat4& pProjMatrix, float pDeltaTime, const std::vector<Mesh*>& pCollisionMeshes)
{
	if (!mIsGrounded)
	{
		mVelocity.y -= 9.81f * pDeltaTime;
	}

	mPos += mVelocity * pDeltaTime;

	mIsGrounded = false;
	for (auto& i : pCollisionMeshes)
	{
		if (mCollider.areCollided(mPlayerHitbox, *i))
		{
			mVelocity.y = 0;
			mIsJumping = false;
			mIsGrounded = true;
		}
	}

	mPlayerHitbox.initMVP(pProjMatrix, mCamera.getViewMatrix(),
		mPos,
		std::make_pair(1.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
		glm::vec3(1.0f, 1.0f, 1.0f));
}

void Player::setCollide(bool pCollides)
{
	mIsCollided = pCollides;
}

Mesh& Player::getHitbox() noexcept
{
	return mPlayerHitbox;
}

Camera& Player::getCamera() noexcept
{
	return mCamera;
}

glm::vec3 Player::getPos() const noexcept
{
	return mPos;
}

glm::vec3 Player::getVelocity() const noexcept
{
	return mVelocity;
}

bool Player::isCollided() const noexcept
{
	return mIsCollided;
}

void Player::solveCollision(const Mesh& pMesh)
{
}