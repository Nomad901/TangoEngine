#pragma once
#include <array>

#include "glm/glm.hpp"
#include "Utils.h"

class FrustumCulling
{
public:
	FrustumCulling() = default;
	FrustumCulling(const glm::mat4& pViewProjMat);

	void update(const glm::mat4& pViewProjMat);

	bool isPointInsideViewFrustum(const glm::vec3& pPoint) const;
	//bool isAABBInsideViewFrustum(const glm::vec3& pMinBounds, 
	//							 const glm::vec3& pMaxBounds) const;

private:
	glm::vec4 mLeftClipPlane;
	glm::vec4 mRightClipPlane;
	glm::vec4 mTopClipPlane;
	glm::vec4 mBottomClipPlane;
	glm::vec4 mNearClipPlane;
	glm::vec4 mFarClipPlane;
};

