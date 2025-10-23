#include "FrustumCulling.h"


FrustumCulling::FrustumCulling(const glm::mat4& pViewProjMat)
{
	update(pViewProjMat);
}

void FrustumCulling::update(const glm::mat4& pViewProjMat)
{
	Utils::getInstance().calculateClipPlanes(mLeftClipPlane, mRightClipPlane, mTopClipPlane,
		mBottomClipPlane, mNearClipPlane, mFarClipPlane,
		pViewProjMat);
}

bool FrustumCulling::isPointInsideViewFrustum(const glm::vec3& pPoint) const
{
	const glm::vec4 pPoint4D(pPoint, 1.0f);

	const bool inside =
		(glm::dot(mLeftClipPlane, pPoint4D) >= 0) &&
		(glm::dot(mRightClipPlane, pPoint4D) <= 0) &&
		(glm::dot(mTopClipPlane, pPoint4D) <= 0) &&
		(glm::dot(mBottomClipPlane, pPoint4D) >= 0) &&
		(glm::dot(mNearClipPlane, pPoint4D) >= 0) &&
		(glm::dot(mFarClipPlane, pPoint4D) <= 0);

	return inside;
}

//FrustumCulling::FrustumCulling()
//{
//	for (size_t i = 0; i < mPlanes.size(); ++i)
//	{
//		mPlanes[i] = Plane();
//	}
//}
//
//FrustumCulling::FrustumCulling(Camera* pCamera, float pAspect, float pFovY, float pZNear, float pZFar)
//{
//	for (size_t i = 0; i < mPlanes.size(); ++i)
//	{
//		mPlanes[i] = Plane();
//	}
//	initFrustumFromCamera(pCamera, pAspect, pFovY, pZNear, pZFar);
//}
//
//void FrustumCulling::initFrustumFromCamera(Camera* pCamera, float pAspect, float pFovY, float pZNear, float pZFar)
//{
//	float halfVSide = pZFar * tanf(pFovY * 0.5f);
//	float halfHSide = halfVSide * pAspect;
//	const glm::vec3 frontMultFar = pZFar * pCamera->getDirection();
//
//	mPlanes[static_cast<int32_t>(PlaneType::NEAR_FACE)] = { pCamera->getPos() + pZNear * pCamera->getDirection(), pCamera->getDirection() };
//	mPlanes[static_cast<int32_t>(PlaneType::FAR_FACE)] = { pCamera->getPos() + frontMultFar, -pCamera->getDirection() };
//	
//	mPlanes[static_cast<int32_t>(PlaneType::RIGHT_FACE)] = { pCamera->getPos(),
//															 glm::cross(frontMultFar - pCamera->getRightVec() * halfHSide, pCamera->getUpVec()) };
//	mPlanes[static_cast<int32_t>(PlaneType::LEFT_FACE)] = { pCamera->getPos(),
//														    glm::cross(pCamera->getUpVec(), frontMultFar + pCamera->getRightVec() * halfHSide) };
//	
//	mPlanes[static_cast<int32_t>(PlaneType::TOP_FACE)] = { pCamera->getPos(),
//														   glm::cross(pCamera->getRightVec(), frontMultFar - pCamera->getUpVec() * halfVSide) };
//	mPlanes[static_cast<int32_t>(PlaneType::BOTTOM_FACE)] = { pCamera->getPos(),
//															  glm::cross(frontMultFar + pCamera->getUpVec() * halfVSide, pCamera->getRightVec()) };
//}
//
Plane& FrustumCulling::getPlane(PlaneType pPlaneType) noexcept
{
	return mPlanes[static_cast<int32_t>(pPlaneType)];
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

const glm::vec3& Plane::getNormal() const noexcept
{
	return mNormal;
}

float Plane::getDistance() const noexcept
{
	return mDistance;
}
/*-----------------------*/
