#include "FrustumCulling.h"


FrustumCulling::FrustumCulling(const glm::mat4& pViewProjMat)
{
	update(pViewProjMat);
}

void FrustumCulling::update(const glm::mat4& pViewProjMat)
{
	const glm::mat4 viewProjMat = glm::transpose(pViewProjMat);
	 
	mPlanes[getIndex(PlaneType::LEFT_FACE)].setClipPlaneVec((viewProjMat[3]   + viewProjMat[0]));
	mPlanes[getIndex(PlaneType::RIGHT_FACE)].setClipPlaneVec((viewProjMat[3]  - viewProjMat[0]));
	mPlanes[getIndex(PlaneType::BOTTOM_FACE)].setClipPlaneVec((viewProjMat[3] + viewProjMat[1]));
	mPlanes[getIndex(PlaneType::TOP_FACE)].setClipPlaneVec((viewProjMat[3]    - viewProjMat[1]));
	mPlanes[getIndex(PlaneType::NEAR_FACE)].setClipPlaneVec((viewProjMat[3]   + viewProjMat[2]));
	mPlanes[getIndex(PlaneType::FAR_FACE)].setClipPlaneVec((viewProjMat[3]    - viewProjMat[2]));
	 
	for (auto& i : mPlanes)
	{
		i.setClipPlaneVec(glm::normalize(i.getClipPlaneVec()));
	}
}

bool FrustumCulling::isPointInsideViewFrustum(const glm::vec3& pPoint) const
{
	const glm::vec4 point4D = glm::vec4(pPoint, 1.0f);

	for (auto& i : mPlanes)
	{
		if (glm::dot(i.getClipPlaneVec(), point4D) < 0.0f)
			return false;
	}
	return true;
}

bool FrustumCulling::isAABBInsideViewFrustum(const glm::vec3& pMinBounds, 
											 const glm::vec3& pMaxBounds) const
{
	for (auto& i : mPlanes)
	{
		const glm::vec4& plane = i.getClipPlaneVec();

		glm::vec3 positivePlane = glm::vec3(plane.x > 0.0f ? pMaxBounds.x : pMinBounds.x,
											plane.y > 0.0f ? pMaxBounds.y : pMinBounds.y,
											plane.z > 0.0f ? pMaxBounds.z : pMinBounds.z);
		
		if (glm::dot(plane, glm::vec4(positivePlane, 1.0f)) < 0.0f)
			return false;
	}
	return true;
}

Plane& FrustumCulling::getPlane(PlaneType pPlaneType) noexcept
{
	return mPlanes[static_cast<int32_t>(pPlaneType)];
}

size_t FrustumCulling::getIndex(PlaneType pPlaneType)
{
	return static_cast<size_t>(pPlaneType);
}

/*-------- PLANE --------*/
Plane::Plane(const glm::vec3& pPoint, const glm::vec3& pNormal)
{
	mNormal = pNormal;
	mDistance = glm::dot(pNormal, pPoint);
}

float Plane::getSignedDistanceToPlane(const glm::vec3& pPoint) const
{
	return glm::dot(mNormal, pPoint) - mDistance;
}

const glm::vec4& Plane::getClipPlaneVec() const noexcept
{
	return mCLipPlaneVec;
}

void Plane::setClipPlaneVec(const glm::vec4& pPlaneVec4)
{
	mCLipPlaneVec = pPlaneVec4;
}

const glm::vec3& Plane::getNormal() const noexcept
{
	return mNormal;
}

float Plane::getDistance() const noexcept
{
	return mDistance;
}
/*-----------------------*/
