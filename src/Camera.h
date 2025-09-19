#pragma once
#include <iostream>
#include <format>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

class Camera
{
public:
	Camera();

	glm::mat4 getViewMatrix();
	void mouseMovement(const glm::vec2& pPos, bool pConstraintPitch = true);
	void moveRight(float pSpeed);
	void moveLeft(float pSpeed);
	void moveForward(float pSpeed);
	void moveBackward(float pSpeed);
	void moveDown(float pSpeed);
	void moveUp(float pSpeed);

	void setSensivity(float pSensivity);
	float getSensivity() const noexcept;

	void setYaw(float pYaw);
	void setPitch(float pPitch);
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
	glm::vec3 mEye, mDirection, mUpVec;

	float mYaw, mPitch;
	float mSensivity;
};

