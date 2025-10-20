#pragma once
#include <iostream>
#include <format>

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Transform
{
public:
	Transform() = default;

	void computeModelMatrix();
	void computeModelMatrix(const glm::mat4& pParent);
	
	void setLocalPosition(const glm::vec3& pPos);
	void setLocalRotation(const glm::vec3& pRotation);
	void setLocalScale(const glm::vec3& pScale);
	
	const glm::mat4& getModelMatrix() const noexcept;
	const glm::vec3& getWorldPosition() const noexcept;
	const glm::vec3& getLocalPosition() const noexcept;
	const glm::vec3& getLocalRotation() const noexcept;
	const glm::vec3& getLocalScale() const noexcept;
	const glm::vec3& getRightVec() const;
	const glm::vec3& getUpVec() const;
	const glm::vec3& getBackwardVec() const;
	const glm::vec3& getForwardVec() const;
	const glm::vec3& getGlobalScale() const;

	bool isShouldBeUpdated() const noexcept;

protected:
	glm::mat4 getLocalMatrix();

private:
	// if the model matrix should be updated - this is true, otherwise this is false
	bool mIsShouldBeRecomputed{ true };

	// localSpace info
	glm::vec3 mPosition{ 0.0f };
	glm::vec3 mRotation{ 0.0f };
	glm::vec3 mScale{ 1.0f };

	// worldSpace info
	glm::mat4 mModelMatrix{ glm::mat4(1.0f) };
};

