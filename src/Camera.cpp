#include "Camera.h"

Camera::Camera()
{
    mEye = glm::vec3(0.0f, 2.0f, 0.0f);
    mDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    mUpVec = glm::vec3(0.0f, 1.0f, 0.0f);

    mYaw = -90.0f;
    mPitch = 0.0f;
    mSensivity = 0.2f;
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(mEye, mEye + mDirection, mUpVec);
}

void Camera::mouseMovement(const glm::vec2& pPos, bool pConstraintPitch)
{
    glm::vec2 mousePos = pPos;
    mousePos.x *= mSensivity;
    mousePos.y *= -mSensivity;
  
    mYaw   += mousePos.x;
    mPitch += mousePos.y;
    
    if (pConstraintPitch)
    {
        if (mPitch > 89.0f)
            mPitch = 89.0f;
        if (mPitch < -89.0f)
            mPitch = -89.0f;
    }

    updateCameraVertex();
}

void Camera::moveCamera(moveSides pMoveSide, float pSpeed, float pDeltaTime)
{
    glm::vec3 tmpVec;
    float velocity = pSpeed * pDeltaTime;

    switch (pMoveSide)
    {
    case moveSides::RIGHT:
        tmpVec = glm::normalize(glm::cross(mDirection, mUpVec));
        mEye += tmpVec * velocity;
        break;
    case moveSides::LEFT:
        tmpVec = glm::normalize(glm::cross(mUpVec, mDirection));
        mEye += tmpVec * velocity;
        break;
    case moveSides::FORWARD:
        if(!mIsInNoclip)
            mEye += glm::vec3(mDirection.x, 0, mDirection.z) * velocity;
        else 
            mEye += mDirection * velocity;
        break;
    case moveSides::BACKWARD:
        if(!mIsInNoclip)
            mEye -= glm::vec3(mDirection.x, 0, mDirection.z) * velocity;
        else 
            mEye -= mDirection * velocity;
        break;
    case moveSides::UP:
        mEye += mUpVec * velocity;
        break;
    case moveSides::DOWN:
        mEye -= mUpVec * velocity;
        break;
    }
} 

void Camera::setMovementSpeed(float pMovementSpeed)
{
    mMovementSpeed = pMovementSpeed;
}

void Camera::setSensivity(float pSensivity)
{
    mSensivity = pSensivity;
}

float Camera::getSensivity() const noexcept
{
    return mSensivity;
}

void Camera::setYaw(float pYaw)
{
    mYaw = pYaw;
}

void Camera::setPitch(float pPitch)
{
    mPitch = pPitch;
}

float Camera::getMovementSpeed() const noexcept
{
    return mMovementSpeed;
}

float Camera::getYaw() const noexcept
{
    return mYaw;
}

float Camera::getPitch() const noexcept
{
    return mPitch;
}

void Camera::turnOnNoclip(bool pNoclip)
{
    mIsInNoclip = pNoclip;
}

void Camera::setPos(const glm::vec3& pPos)
{
    mEye = pPos;
}

void Camera::setDirection(const glm::vec3& pDirection)
{
    mDirection = pDirection;
}

void Camera::setUpVec(const glm::vec3& pUpVec)
{
    mUpVec = pUpVec;
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

glm::vec3 Camera::getRightVec() const noexcept
{
    return glm::normalize(glm::cross(mDirection, mUpVec));
}

glm::vec3 Camera::getLeftVec() const noexcept
{
    return glm::normalize(glm::cross(mUpVec, mDirection));
}

void Camera::updateCameraVertex()
{
    glm::vec3 front;
    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front = glm::normalize(front);
    mDirection = glm::normalize(front);
}
