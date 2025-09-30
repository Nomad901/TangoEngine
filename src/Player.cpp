#include "Player.h"

Player::Player(const glm::vec3& pPos, const glm::vec3& pVelocity, float pSpeed, 
			   float pSprintVelocity, float pJumpForce, bool pThirdPersonCam)
{
	init(pPos, pVelocity, pSpeed, pSprintVelocity, pJumpForce, pThirdPersonCam);
}

void Player::init(const glm::vec3& pPos, const glm::vec3& pVelocity, float pSpeed, 
				  float pSprintVelocity, float pJumpForce, bool pThirdPersonCam)
{
	mJumpForce = pJumpForce;
	mMoveSpeed = pSpeed;
	mPos = pPos;
	mVelocity = pVelocity;
	mSprintSpeed = pSprintVelocity;
	m3rdPersonCamera = pThirdPersonCam;
	std::shared_ptr<Primitive> hitbox = std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	std::weak_ptr weakHitbox = hitbox;
	mPlayerHitbox.init(hitbox);
	mPlayerHitbox.setPos(mPos);
	mCamera.setPos(mPos);
	mThirdPersonCam.setPos(mPos);
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

	if (!mIsFreezed)
	{
		switch (pMoveSidesPlayer)
		{
		case moveSidesPlayer::RIGHT:
			if (!m3rdPersonCamera)
				mCamera.moveCamera(moveSides::RIGHT, speed, pDeltaTime);
			mPos = mCamera.getPos();
			break;
		case moveSidesPlayer::LEFT:
			if (!m3rdPersonCamera)
				mCamera.moveCamera(moveSides::LEFT, speed, pDeltaTime);
			mPos = mCamera.getPos();
			break;
		case moveSidesPlayer::FORWARD:
			if (!m3rdPersonCamera)
				mCamera.moveCamera(moveSides::FORWARD, speed, pDeltaTime);
			mPos = mCamera.getPos();
			break;
		case moveSidesPlayer::BACKWARD:
			if (!m3rdPersonCamera)
				mCamera.moveCamera(moveSides::BACKWARD, speed, pDeltaTime);
			mPos = mCamera.getPos();
			break;
		}
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

void Player::update(const glm::mat4& pProjMatrix, float pDeltaTime, const std::vector<Mesh*>& pCollisionMeshes,
					Shader& pShader, SDL_Event& pEvents)
{
	checkCollisions(pCollisionMeshes);
	if(mNoclip)
		mCamera.turnOnNoclip(mNoclip);
	glm::mat4 viewMatrix = glm::mat4(0.0f);
	if (!m3rdPersonCamera)
	{
		mCamera.setPos(mPos);
		viewMatrix = mCamera.getViewMatrix();
	}
	else
	{
		mThirdPersonCam.update(pEvents, mPos);
		mThirdPersonCam.setPos(mPos);
		viewMatrix = mThirdPersonCam.getViewMatrix();
	}
	mPlayerHitbox.initMVP(pProjMatrix, viewMatrix,
		mPos,
		std::make_pair(1.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
		glm::vec3(10.0f, 30.0f, 10.0f));
	mPlayerHitbox.draw(pShader);
}

void Player::checkCollisions(const std::vector<Mesh*>& pCollisionMeshes)
{
	for (auto& i : pCollisionMeshes)
	{
		if (mCollider.areCollided(*i, mPlayerHitbox))
		{
			responseCollision(*i);
		}
	}
}

void Player::turnOnNoclip(bool pNoclip)
{
	mNoclip = pNoclip;
}

void Player::freezePlayer(bool pFreeze)
{
	mIsFreezed = pFreeze;
}

void Player::turnOn3rdPersonCamera(bool p3rdPersonCamera)
{
	m3rdPersonCamera = p3rdPersonCamera;
}

void Player::setPos(const glm::vec3& pPos)
{
	mPos = pPos;
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

void Player::responseCollision(Mesh& pObstacle)
{
	const glm::vec3 obstacleMinPoint = mCollider.getMinPoint(pObstacle.getPos(), pObstacle.getSize());
	const glm::vec3 obstacleMaxPoint = mCollider.getMaxPoint(pObstacle.getPos(), pObstacle.getSize());
	const glm::vec3 playerMinPoint = mCollider.getMinPoint(mPlayerHitbox.getPos(), mPlayerHitbox.getSize());
	const glm::vec3 playerMaxPoint = mCollider.getMaxPoint(mPlayerHitbox.getPos(), mPlayerHitbox.getSize());
	
	const glm::vec3 overlap(
		std::min(obstacleMaxPoint.x, playerMaxPoint.x) - std::max(obstacleMinPoint.x, playerMinPoint.x),
		std::min(obstacleMaxPoint.y, playerMaxPoint.y) - std::max(obstacleMinPoint.y, playerMinPoint.y),
		std::min(obstacleMaxPoint.z, playerMaxPoint.z) - std::max(obstacleMinPoint.z, playerMinPoint.z)
	);

	if (overlap.x < overlap.y && overlap.x < overlap.z)
	{
		if (mPlayerHitbox.getPos().x < pObstacle.getPos().x)
			mPos.x -= overlap.x;
		else
			mPos.x += overlap.x;
	}
	else if (overlap.x < overlap.z)
	{
		if (mPlayerHitbox.getPos().y < pObstacle.getPos().y)
			mPos.y -= overlap.y;
		else
		{
			mPos.y += overlap.y;
			mVelocity.y = 0.0f;
			mIsGrounded = true;
		}
	}
	else
	{
		if (mPlayerHitbox.getPos().z < pObstacle.getPos().z)
			mPos.z -= overlap.z;
		else
			mPos.z += overlap.z;
	}
}
