#include "Camera.h"

Camera::Camera()
{
    mEye = glm::vec3(0.0f, 2.0f, 0.0f);
    mDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    mUpVec = glm::vec3(0.0f, 1.0f, 0.0f);

    mYaw = -90.0f;
    mPitch = 0.0f;
    mSensivity = 45.0f;
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(mEye, mEye + mDirection, mUpVec);
}

void Camera::mouseMovement(const glm::vec2& pPos, bool pConstraintPitch)
{
    glm::vec2 mousePos = pPos;
    mousePos.x *= mSensivity;
    mousePos.y *= mSensivity;

    mYaw   += mousePos.x;
    mPitch += mousePos.y;
    
    if (pConstraintPitch)
    {
        if (mPitch > 89.0f)
            mPitch = 89.0f;
        if (mYaw < -89.0f)
            mYaw = -89.0f;
    }

    updateCameraVertex();
    
    //mDirection = glm::rotate(mDirection, glm::radians(-pPos.x), mUpVec);
    //glm::vec3 mTmpVec = glm::normalize(glm::cross(mDirection, mUpVec));
    //mDirection = glm::rotate(mDirection, glm::radians(-pPos.y), mTmpVec);
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

float Camera::getYaw() const noexcept
{
    return mYaw;
}

float Camera::getPitch() const noexcept
{
    return mPitch;
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

void Camera::updateCameraVertex()
{
    glm::vec3 front;
    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front = glm::normalize(front);


}
