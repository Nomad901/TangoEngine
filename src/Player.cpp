#include "Player.h"

Player::Player(const glm::vec3& pPos, const glm::vec3& pVelocity, float pSpeed, 
			   float pSprintVelocity, float pJumpForce)
{
	init(pPos, pVelocity, pSpeed, pSprintVelocity, pJumpForce);
}

void Player::init(const glm::vec3& pPos, const glm::vec3& pVelocity, float pSpeed, 
				  float pSprintVelocity, float pJumpForce)
{
	mJumpForce = pJumpForce;
	mMoveSpeed = pSpeed;
	mPos = pPos;
	mVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	mSprintSpeed = pSprintVelocity;
	std::shared_ptr<Primitive> hitbox = std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	std::weak_ptr weakHitbox = hitbox;
	mPlayerHitbox.init(hitbox);
}

void Player::move(moveSidesPlayer pMoveSidesPlayer, float pDeltaTime)
{
	float speed = mIsSprinting ? mSprintSpeed : mMoveSpeed;

	switch (pMoveSidesPlayer)
	{
	case moveSidesPlayer::RIGHT:
		mCamera.moveCamera(moveSides::RIGHT, speed, pDeltaTime);
		mPos = mCamera.getPos();
		mInputVelocity.x += speed;
		break;
	case moveSidesPlayer::LEFT:
		mCamera.moveCamera(moveSides::LEFT, speed, pDeltaTime);
		mPos = mCamera.getPos();
		mInputVelocity.x -= speed;
		break;
	case moveSidesPlayer::FORWARD:
		mCamera.moveCamera(moveSides::FORWARD, speed, pDeltaTime);
		mPos = mCamera.getPos();
		mInputVelocity.z -= speed;
		break;
	case moveSidesPlayer::BACKWARD:
		mCamera.moveCamera(moveSides::BACKWARD, speed, pDeltaTime);
		mPos = mCamera.getPos();
		mInputVelocity.z += speed;
		break;
	}
}

void Player::jump()
{
	if (mIsGrounded)
	{
		mPhysicsVelocity.y = mJumpForce;
		mIsGrounded = false;
	}
}

void Player::sprint(bool pSprint)
{
	mIsSprinting = pSprint;
}

void Player::update(const glm::mat4& pProjMatrix, float pDeltaTime, const std::vector<Mesh*>& pCollisionMeshes)
{
	mInputVelocity *= std::pow(mFriction, pDeltaTime * 60.0f);

	if (!mIsGrounded)
		mPhysicsVelocity -= mGravity * pDeltaTime;

	mCamera.setPos(mPos);
	mPlayerHitbox.initMVP(pProjMatrix, mCamera.getViewMatrix(),
		mPos,
		std::make_pair(1.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
		glm::vec3(1.0f, 1.0f, 1.0f));
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

float Player::getSpeed() const noexcept
{
	return mMoveSpeed;
}

float Player::getSprintSpeed() const noexcept
{
	return mSprintSpeed;
}

void Player::solveCollision(const Mesh& pMesh)
{
}