#include "Transform.h"

void Transform::computeModelMatrix()
{
	mModelMatrix = getLocalMatrix();
	mIsShouldBeRecomputed = false;
}

void Transform::computeModelMatrix(const glm::mat4& pParent)
{
	mModelMatrix = pParent * getLocalMatrix();
	mIsShouldBeRecomputed = false;
}

void Transform::setLocalPosition(const glm::vec3& pPos)
{
	mPosition = pPos;
	mIsShouldBeRecomputed = true;
}

void Transform::setLocalRotation(const glm::vec3& pRotation)
{
	mRotation = pRotation;
	mIsShouldBeRecomputed = true;
}

void Transform::setLocalScale(const glm::vec3& pScale)
{
	mScale = pScale;
	mIsShouldBeRecomputed = true;
}

const glm::mat4& Transform::getModelMatrix() const noexcept
{
	return mModelMatrix;
}

const glm::vec3& Transform::getWorldPosition() const noexcept
{
	return mModelMatrix[3];
}

const glm::vec3& Transform::getLocalPosition() const noexcept
{
	return mPosition;
}

const glm::vec3& Transform::getLocalRotation() const noexcept
{
	return mRotation;
}

const glm::vec3& Transform::getLocalScale() const noexcept
{
	return mScale;
}

const glm::vec3& Transform::getRightVec() const
{
	return mModelMatrix[0];
}

const glm::vec3& Transform::getUpVec() const
{
	return mModelMatrix[1];
}

const glm::vec3& Transform::getBackwardVec() const
{
	return mModelMatrix[2];
}

const glm::vec3& Transform::getForwardVec() const
{
	return -mModelMatrix[2];
}

const glm::vec3& Transform::getGlobalScale() const
{
	return glm::vec3(glm::length(getRightVec()), glm::length(getUpVec()), glm::length(getBackwardVec()));
}

bool Transform::isShouldBeUpdated() const noexcept
{
	return mIsShouldBeRecomputed;
}

glm::mat4 Transform::getLocalMatrix()
{
	const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(mRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(mRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(mRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	const glm::mat4 rotationMatrix = transformX * transformY * transformZ;

	return glm::translate(glm::mat4(1.0f), mPosition) * rotationMatrix * glm::scale(glm::mat4(1.0f), mScale);
}
