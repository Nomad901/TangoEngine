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
	mCurrentState = states::JUMPING_UP;
}

void Player::sprint(bool pSprint)
{
	mIsSprinting = pSprint;
}

void Player::update(const glm::mat4& pProjMatrix, float pDeltaTime, const std::vector<Mesh*>& pCollisionMeshes)
{
	//mInputVelocity *= std::pow(mFriction, pDeltaTime * 60.0f);

	//if (!mIsGrounded)
	//	mPhysicsVelocity.y -= mGravity * pDeltaTime;
	//
	//glm::vec3 totalVelocity = mInputVelocity + mPhysicsVelocity;

	//glm::vec3 newPos = mPos + mVelocity * pDeltaTime;

	//mPos = newPos;

	if (mCurrentState == states::JUMPING_UP)
		mPos.y += mJumpForce;
	if (mPos.y >= 40.0f)
		mCurrentState = states::JUMPING_DOWN;
	if (mCurrentState == states::JUMPING_DOWN)
		mPos.y -= mJumpForce;
	if (mPos.y <= 2.0f)
		mCurrentState = states::STANDING;

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
