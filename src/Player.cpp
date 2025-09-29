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
	mVelocity = pVelocity;
	mSprintSpeed = pSprintVelocity;
	std::shared_ptr<Primitive> hitbox = std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	std::weak_ptr weakHitbox = hitbox;
	mPlayerHitbox.init(hitbox);
	mPlayerHitbox.setPos(mPos);
	mCamera.setPos(mPos);
}

void Player::move(moveSidesPlayer pMoveSidesPlayer, float pDeltaTime)
{
	//const float ACCELERATION_RATIO = 0.1f;
	//glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);

	//switch (pMoveSidesPlayer)
	//{
	//case moveSidesPlayer::RIGHT:
	//	direction += mCamera.getRightVec();
	//	break;
	//case moveSidesPlayer::LEFT:
	//	direction -= mCamera.getRightVec();
	//	break;
	//case moveSidesPlayer::FORWARD:
	//	direction += mCamera.getDirection();
	//	break;
	//case moveSidesPlayer::BACKWARD:
	//	direction -= mCamera.getDirection();
	//	break;
	//}

	//if (glm::length(direction) > 0.0f)
	//{
	//	direction = glm::normalize(direction);
	//	//					 move-speed 
	//	const auto finalSpeed = 2.5f * ACCELERATION_RATIO * pDeltaTime;
	//	mVelocity += finalSpeed * direction;
	//}

	float speed = mIsSprinting ? mSprintSpeed : mMoveSpeed;

	switch (pMoveSidesPlayer)
	{
	case moveSidesPlayer::RIGHT:
		mCamera.moveCamera(moveSides::RIGHT, speed, pDeltaTime);
		mPos = mCamera.getPos();
		break;
	case moveSidesPlayer::LEFT:
		mCamera.moveCamera(moveSides::LEFT, speed, pDeltaTime);
		mPos = mCamera.getPos();
		break;
	case moveSidesPlayer::FORWARD:
		mCamera.moveCamera(moveSides::FORWARD, speed, pDeltaTime);
		mPos = mCamera.getPos();
		break;
	case moveSidesPlayer::BACKWARD:
		mCamera.moveCamera(moveSides::BACKWARD, speed, pDeltaTime);
		mPos = mCamera.getPos();
		break;
	}
}

void Player::jump()
{
	if (mIsGrounded)
	{
		mVelocity.y = mJumpForce;
	}
}

void Player::sprint(bool pSprint)
{
	mIsSprinting = pSprint;
}

void Player::update(const glm::mat4& pProjMatrix, float pDeltaTime, const std::vector<Mesh*>& pCollisionMeshes)
{

	mCamera.turnOnNoclip(mNoclip);
	mCamera.setPos(mPos);
	mPlayerHitbox.initMVP(pProjMatrix, mCamera.getViewMatrix(),
		mPos,
		std::make_pair(1.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
		glm::vec3(1.0f, 1.0f, 1.0f));
}

void Player::turnOnNoclip(bool pNoclip)
{
	mNoclip = pNoclip;
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

bool Player::isOnGround() const noexcept
{
	return mPos.y <= 0.0f || mIsGrounded;
}
