#include "Camera.h"

Camera::Camera()
{
    mEye = glm::vec3(0.0f, 2.0f, 0.0f);
    mDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    mUpVec = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(mEye, mEye + mDirection, mUpVec);
}

void Camera::mouseMovement(const glm::vec2& pPos)
{
    mDirection = glm::rotate(mDirection, glm::radians(-pPos.x), mUpVec);
    glm::vec3 mTmpVec = glm::normalize(glm::cross(mDirection, mUpVec));
    mDirection = glm::rotate(mDirection, glm::radians(-pPos.y), mTmpVec);
}

void Camera::moveRight(float pSpeed)
{
    glm::vec3 tmpVec = glm::normalize(glm::cross(mDirection, mUpVec));
    mEye += tmpVec * pSpeed;
}

void Camera::moveLeft(float pSpeed)
{
    glm::vec3 tmpVec = glm::normalize(glm::cross(mUpVec, mDirection));
    mEye += tmpVec * pSpeed;
}

void Camera::moveForward(float pSpeed)
{
    mEye += mDirection * pSpeed;
}

void Camera::moveBackward(float pSpeed)
{
    mEye -= mDirection * pSpeed;
}

void Camera::moveDown(float pSpeed)
{
    mEye -= mUpVec * pSpeed;
}

void Camera::moveUp(float pSpeed)
{
    mEye += mUpVec * pSpeed;
}

glm::vec3 Camera::getPos() const noexcept
{
    return mEye;
}

glm::vec3 Camera::getDirection() const noexcept
{
    return mDirection;
}

glm::vec3 Camera::getUpVec() const noexcept
{
    return mUpVec;
}
