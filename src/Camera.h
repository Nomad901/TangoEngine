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
	void mouseMovement(const glm::vec2& pPos);
	void moveRight(float pSpeed);
	void moveLeft(float pSpeed);
	void moveForward(float pSpeed);
	void moveBackward(float pSpeed);
	void moveDown(float pSpeed);
	void moveUp(float pSpeed);

	glm::vec3 getPos() const noexcept;
	glm::vec3 getDirection() const noexcept;
	glm::vec3 getUpVec() const noexcept;

private:
	glm::vec3 mEye, mDirection, mUpVec;
};

