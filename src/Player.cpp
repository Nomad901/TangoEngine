#include "Player.h"

Player::Player(const glm::vec3& pPos, const glm::vec3& pVelocity, float pSpeed, 
			   float pSprintVelocity, float pJumpForce, bool pThirdPersonCam, 
			   const std::filesystem::path& pPath)
{
	init(pPos, pVelocity, pSpeed, pSprintVelocity, pJumpForce, pThirdPersonCam, pPath);
}

void Player::init(const glm::vec3& pPos, const glm::vec3& pVelocity, float pSpeed, 
				  float pSprintVelocity, float pJumpForce, bool pThirdPersonCam, 
				  const std::filesystem::path& pPath)
{
	//mCharModel.initOBJmodel(pPos, pPath);

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
	if (mIsFreezed)
		return;

	float speed = mIsSprinting ? mSprintSpeed : mMoveSpeed;

	if (m3rdPersonCamera)
	{
		glm::vec3 moveDirection{ 0.0f };

		glm::vec3 cameraForward;
		if(!mNoclip)
			cameraForward = glm::normalize(glm::vec3(mThirdPersonCam.getDirection().x, 0.0f, mThirdPersonCam.getDirection().z));
		else 
			cameraForward = glm::normalize(glm::vec3(mThirdPersonCam.getDirection().x, mThirdPersonCam.getDirection().y, mThirdPersonCam.getDirection().z));
		glm::vec3 cameraRight = glm::normalize(glm::cross(cameraForward, glm::vec3(0.0f, 1.0f, 0.0f)));

		switch (pMoveSidesPlayer)
		{
		case moveSidesPlayer::FORWARD_:
			moveDirection = cameraForward;
			break;
		case moveSidesPlayer::BACKWARD_:
			moveDirection = -cameraForward;
			break;
		case moveSidesPlayer::RIGHT_:
			moveDirection = cameraRight;
			break;
		case moveSidesPlayer::LEFT_:
			moveDirection = -cameraRight;
			break;
		}

		if(!mNoclip)
			moveDirection.y = 0.0f;

		if (glm::length(moveDirection) > 0.0f)
		{
			moveDirection = glm::normalize(moveDirection);
			mPos += moveDirection * speed * pDeltaTime;

			mRotationY = glm::degrees(atan2f(-cameraForward.x, -cameraForward.z));
		}
	}
	else
	{
		switch (pMoveSidesPlayer)
		{
		case moveSidesPlayer::RIGHT_:
			mCamera.moveCamera(moveSides::cam_RIGHT, speed, pDeltaTime);
			mPos = mCamera.getPos();
			break;
		case moveSidesPlayer::LEFT_:
			mCamera.moveCamera(moveSides::cam_LEFT, speed, pDeltaTime);
			mPos = mCamera.getPos();
			break;
		case moveSidesPlayer::FORWARD_:
			mCamera.moveCamera(moveSides::cam_FORWARD, speed, pDeltaTime);
			mPos = mCamera.getPos();
			break;
		case moveSidesPlayer::BACKWARD_:
			mCamera.moveCamera(moveSides::cam_BACKWARD, speed, pDeltaTime);
			mPos = mCamera.getPos();
			break;
		}
	}
}

void Player::jump()
{
	if (mPos.y <= mTerrainHeight)
	{
		mPos.y += mJumpForce;
		mIsGrounded = false;
	}
}

void Player::sprint(bool pSprint)
{
	mIsSprinting = pSprint;
}

void Player::turnOnRotatingWithCharacter(bool pRotatingWithChar)
{
	mRotateCameraWithChar = pRotatingWithChar;
}

void Player::update(const glm::mat4& pProjMatrix, float pDeltaTime, const std::vector<Mesh*>& pCollisionMeshes,
					Terrain* pTerrain)
{
	mPos = pTerrain->getCameraPosForChar(mPos, 10.0f);

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
		mThirdPersonCam.update(mEvents, mPos, mRotationY);
		viewMatrix = mThirdPersonCam.getViewMatrix();
	}
	mPlayerHitbox.initMVP(pProjMatrix, viewMatrix,
						  mPos,
						  std::make_pair(mRotationY, glm::vec3(0.0f, 1.0f, 0.0f)),
						  glm::vec3(10.0f, 20.0f, 10.0f));
	//mCharModel.initMVP(pProjMatrix, viewMatrix,
	//				     mPos,
	//				     std::make_pair(mRotationY, glm::vec3(0.0f, 1.0f, 0.0f)),
	//				     glm::vec3(30.0f, 30.0f, 30.0f));
}	

void Player::renderCharacter(Shader& pShader)
{
	glDisable(GL_CULL_FACE);
	pShader.bind();
	if (m3rdPersonCamera)
	{
		//mCharModel.render(pShader);
		mPlayerHitbox.draw(pShader);
	}
	glEnable(GL_CULL_FACE);
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

void Player::updateEvents(const SDL_Event& pEvent)
{
	mEvents = pEvent;
}

Mesh& Player::getHitbox() noexcept
{
	return mPlayerHitbox;
}

Camera& Player::getCamera() noexcept
{
	return mCamera;
}

thirdPersonCam& Player::getThirdPersonCamera() noexcept
{
	return mThirdPersonCam;
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

float Player::getRotationY() const noexcept
{
	return mRotationY;
}

bool Player::ifRotatesWithCamera() const noexcept
{
	return mRotateCameraWithChar;
}

bool Player::isInThirdPersonCamera() const noexcept
{
	return m3rdPersonCamera;
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
		std::min(obstacleMaxPoint.z, playerMaxPoint.z) - std::max(obstacleMinPoint.z, playerMinPoint.z));
	
	if (overlap.x < overlap.y && overlap.x < overlap.z)
	{
		if (mPlayerHitbox.getPos().x < pObstacle.getPos().x)
			mPos.x -= overlap.x;
		else
			mPos.x += overlap.x;
	}
	else if (overlap.y < overlap.z)
	{
		if (mPlayerHitbox.getPos().y < pObstacle.getPos().y)
			mPos.y -= overlap.y;
		else
		{
			mPos.y += overlap.y;
			mVelocity.y = 0.0f;
			mTerrainHeight = mPos.y;
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
