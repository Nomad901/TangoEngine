#pragma once
#include <iostream>
#include <format>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

enum class moveSides
{
	RIGHT = 0,
	LEFT = 1,
	FORWARD = 2,
	BACKWARD = 3,
	UP = 4,
	DOWN = 5
};

class Camera
{
public:
	Camera();

	glm::mat4 getViewMatrix();
	void mouseMovement(const glm::vec2& pPos, bool pConstraintPitch = true);
	void moveCamera(moveSides pMoveSide, float pSpeed, float pDeltaTime);

	void setMovementSpeed(float pMovementSpeed);
	void setSensivity(float pSensivity);
	void setYaw(float pYaw);
	void setPitch(float pPitch);
	float getMovementSpeed() const noexcept;
	float getSensivity() const noexcept;
	float getYaw() const noexcept;
	float getPitch() const noexcept;

	void setPos(const glm::vec3& pPos);
	void setDirection(const glm::vec3& pDirection);
	void setUpVec(const glm::vec3& pUpVec);
	glm::vec3 getPos() const noexcept;
	glm::vec3 getDirection() const noexcept;
	glm::vec3 getUpVec() const noexcept;

private:
	void updateCameraVertex();

private:
	float mYaw{}, mPitch{};
	float mSensivity{};
	float mMovementSpeed{};

	glm::vec3 mEye, mDirection, mUpVec;
};

