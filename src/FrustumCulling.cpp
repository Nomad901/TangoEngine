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
		(glm::dot(mLeftClipPlane, pPoint4D)   >= 0) &&
		(glm::dot(mRightClipPlane, pPoint4D)  <= 0) &&
		(glm::dot(mTopClipPlane, pPoint4D)	  <= 0) &&
		(glm::dot(mBottomClipPlane, pPoint4D) >= 0) &&
		(glm::dot(mNearClipPlane, pPoint4D)	  >= 0) &&
		(glm::dot(mFarClipPlane, pPoint4D)	  <= 0);

	return inside;
}

//bool FrustumCulling::isAABBInsideViewFrustum(const glm::vec3& pMinBounds, const glm::vec3& pMaxBounds) const
//{
//	//for (int i = 0; i < mPlanes.size(); ++i) 
//	//{
//	//	const glm::vec4& plane = mPlanes[i];
//	//
//	//	glm::vec3 positiveVertex = pMinBounds;
//	//	if (plane.x >= 0) positiveVertex.x = pMaxBounds.x;
//	//	if (plane.y >= 0) positiveVertex.y = pMaxBounds.y;
//	//	if (plane.z >= 0) positiveVertex.z = pMaxBounds.z;
//	//
//	//	if (glm::dot(glm::vec3(plane), positiveVertex) + plane.w < 0) 
//	//		return false; 
//	//}
//	//return true;
//}
